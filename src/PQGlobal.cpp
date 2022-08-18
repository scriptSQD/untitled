#include <PQGlobal.hpp>

std::pair<bool, std::string>
PQGlobal::MakePQConnection(const std::string &connString) {
    try {
        SQD_LOG(fmt::format("Attempting to connect to PostgreSQL at '{}'",
                            connString));
        PQGlobal::s_PGconnection =
            std::make_unique<pqxx::connection>(connString);

        SQD_LOG("Connected successfully! Processing associated callbacks.");
        for (const auto &cb : PQGlobal::s_SucceedCallbackList) {
            std::invoke(cb);
        }

        return {true, ""};
    } catch (...) {
        SQD_ERR("Caught exception while trying to connect to PGSQL.");
        std::string what =
            ExceptionHandler::HandleEptr(std::current_exception());

        SQD_LOG("Processing callbacks on failure.");
        for (const auto &cb : PQGlobal::s_FailedCallbackList) {
            std::invoke(cb);
        }

        return {false, what};
    }
}
bool PQGlobal::CloseConnection() {
    if (!PQGlobal::s_PGconnection)
        return false;

    try {
        PQGlobal::s_PGconnection->close();
        return true;
    } catch (...) {
        ExceptionHandler::HandleEptr(std::current_exception());
        return false;
    }
}

std::optional<pqxx::result> PQGlobal::ProcessQuery(std::string_view query) {
    auto t = PQGlobal::CreateTransaction();

    if (!t) {
        SQD_WARN("Database transaction not initialized (refs. nullptr)!");
        return {};
    }

    SQD_LOG("Processing query: " + std::string(query));
    try {
        auto res = t->exec(query);
        t->commit();
        return res;
    } catch (...) {
        SQD_ERR("Caught exception while trying to execute and commit "
                "database transaction!");
        ExceptionHandler::HandleEptr(std::current_exception());
        return {};
    }
}

std::vector<std::string>
PQGlobal::EnumerateColumns(std::string_view tableName) {
    auto resp = std::vector<std::string>();

    const auto &q =
        fmt::format("SELECT column_name FROM information_schema.columns WHERE "
                    "table_name = N'{}'",
                    tableName);

    auto queryRes = PQGlobal::ProcessQuery(q);

    Utils::HandleOptional(queryRes, [&resp](const pqxx::result &queryRes) {
        auto index = 0;
        try {
            for (pqxx::row row : queryRes) {
                const auto [rowName] = row.as<std::string>();

                resp.emplace_back(rowName);
                index++;
            }
        } catch (...) {
            SQD_ERR("Caught exception while trying to populate with enumerated "
                    "columns.");
            ExceptionHandler::HandleEptr(std::current_exception());
        }
    });

    return resp;
}
DatabaseTable PQGlobal::ParseTable(std::string_view schemaName,
                                   std::string_view tableName) {
    auto ret = DatabaseTable();

    auto cols = EnumerateColumns(tableName);
    for (const auto &col : cols) {
        // Populate column metadata
        ret.InsertColumn(col);
    }

    auto queryResp = ProcessQuery(
        fmt::format(R"(SELECT * FROM "{}"."{}")", schemaName, tableName));

    Utils::HandleOptional(queryResp, [&ret,
                                      &cols](const pqxx::result &queryRes) {
        for (pqxx::row row : queryRes) {
            auto rowData = DatabaseTable::RowData();

            // Loop through columns and accumulate data
            for (const auto &col : cols) {
                std::string colVal;
                row.at(col).to(colVal, std::string("<null>"));

                rowData.emplace_back(DatabaseTable::ColumnData({col, colVal}));
            }

            ret.InsertRow(rowData);
        }
    });

    return ret;
}

DatabaseMetadata PQGlobal::IntrospectDatabase() {
    auto dbMeta = DatabaseMetadata();

    for (const auto &schema : PQGlobal::GetSchemas()) {
        dbMeta.AddSchema(schema);

        for (const auto &table : PQGlobal::GetTablesForSchema(schema)) {
            dbMeta.AddTableForSchema(schema, table);
        }
    }

    return dbMeta;
}

std::vector<std::string> PQGlobal::GetSchemas() {
    auto ret = std::vector<std::string>();

    auto queryResp =
        PQGlobal::ProcessQuery("SELECT schema_name\n"
                               "FROM information_schema.schemata\n"
                               "WHERE schema_name !~ '^pg_'\n"
                               "AND NOT schema_name = 'information_schema'");

    Utils::HandleOptional(queryResp, [&ret](const pqxx::result &queryRes) {
        for (pqxx::row row : queryRes) {
            const auto &[schemaName] = row.as<std::string>();

            ret.emplace_back(schemaName);
        }
    });

    return ret;
}

std::vector<std::string> PQGlobal::GetTablesForSchema(std::string_view name) {
    auto ret = std::vector<std::string>();

    auto queryResp = PQGlobal::ProcessQuery(
        fmt::format("SELECT table_name FROM information_schema.tables\n"
                    "WHERE table_schema = '{}'",
                    name));

    Utils::HandleOptional(queryResp, [&ret](const pqxx::result &queryRes) {
        for (pqxx::row row : queryRes) {
            const auto &[schemaName] = row.as<std::string>();

            ret.emplace_back(schemaName);
        }
    });

    return ret;
}
