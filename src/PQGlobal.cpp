#include <PQGlobal.hpp>

std::pair<int, std::string>
PQGlobal::AddConnection(const std::string &connectionString,
                        const std::string &displayName) {
    const auto &connId = s_LastConnectionIdentifier + 1;
    try {
        SQD_LOG(fmt::format("Attempting to connect to PostgreSQL at '{}'",
                            connectionString));

        s_ActiveConnections.insert(
            {connId, std::make_unique<pqxx::connection>(connectionString)});

        s_ConnectionsInfoMap.insert({connId, displayName});

        SQD_LOG("Connected successfully! Processing associated callbacks.");
        for (const auto &cb : PQGlobal::s_SucceedCallbackList) {
            SQD_LOG("Invoking callback on success");
            std::invoke(cb, connId, displayName);
        }

        s_LastConnectionIdentifier++;

        return {connId, ""};
    } catch (...) {
        SQD_ERR("Caught exception while trying to connect to PGSQL.");
        std::string what =
            ExceptionHandler::HandleEptr(std::current_exception());

        SQD_LOG("Processing callbacks on failure.");
        for (const auto &cb : PQGlobal::s_FailedCallbackList) {
            SQD_LOG("Invoking callback on failure");
            std::invoke(cb);
        }

        return {-1, what};
    }
}

std::pair<bool, std::string>
PQGlobal::TestConnection(const std::string &connectionString) {
    try {
        SQD_LOG(
            fmt::format("Testing connection to PG via {}.", connectionString));
        std::make_unique<pqxx::connection>(connectionString);

        SQD_LOG("Connected successfully!");
        return {true, ""};
    } catch (...) {
        return {false, ExceptionHandler::HandleEptr(std::current_exception())};
    }
}

bool PQGlobal::CloseConnection(int identifier) {
    // Executed before connection closing happens
    for (const auto &cb : s_ConnectionCloseCallbacks) {
        std::invoke(cb, identifier);
    }

    try {
        s_ActiveConnections.at(identifier)->close();
        s_ActiveConnections.erase(identifier);

        for (const auto &cb : s_ConnectionClosedCallbacks) {
            std::invoke(cb, identifier);
        }

        s_ConnectionsInfoMap.erase(identifier);

        return true;
    } catch (...) {
        ExceptionHandler::HandleEptr(std::current_exception());
        return false;
    }
}

bool PQGlobal::CloseAllConnections() {
    bool ret = false;

    for (int i = 1; i < GetLastConnectionId(); i++) {
        try {
            s_ActiveConnections.at(i)->close();
            ret = true;
        } catch (...) {
            ExceptionHandler::HandleEptr(std::current_exception());
            return false;
        }
    }

    return ret;
}

std::optional<pqxx::result> PQGlobal::ProcessQuery(int connectionIdentifier,
                                                   std::string_view query) {
    auto t = PQGlobal::CreateTransaction(connectionIdentifier);

    return Utils::HandleOptional<std::optional<pqxx::result>>(
        t, [&query](const std::shared_ptr<pqxx::work> &t) {
            SQD_LOG("Processing query: " + std::string(query));
            auto ret = std::optional<pqxx::result>();

            try {
                auto res = t->exec(query);
                t->commit();

                ret = res;
                return ret;
            } catch (...) {
                SQD_ERR("Caught exception while trying to execute and commit "
                        "database transaction!");
                ExceptionHandler::HandleEptr(std::current_exception());
                return ret;
            }
        });
}

std::vector<std::string>
PQGlobal::EnumerateColumns(int connectionIdentifier,
                           std::string_view tableName) {
    auto resp = std::vector<std::string>();

    const auto &q =
        fmt::format("SELECT column_name FROM information_schema.columns WHERE "
                    "table_name = N'{}'",
                    tableName);

    auto queryRes = PQGlobal::ProcessQuery(connectionIdentifier, q);

    Utils::HandleOptional<void>(queryRes, [&resp](
                                              const pqxx::result &queryRes) {
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
DatabaseTable PQGlobal::ParseTable(int connectionIdentifier,
                                   std::string_view schemaName,
                                   std::string_view tableName) {
    auto ret = DatabaseTable();

    auto cols = EnumerateColumns(connectionIdentifier, tableName);
    for (const auto &col : cols) {
        // Populate column metadata
        ret.InsertColumn(col);
    }

    auto queryResp = ProcessQuery(
        connectionIdentifier,
        fmt::format(R"(SELECT * FROM "{}"."{}")", schemaName, tableName));

    Utils::HandleOptional<void>(queryResp, [&ret, &cols](
                                               const pqxx::result &queryRes) {
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

DatabaseMetadata PQGlobal::IntrospectDatabase(int connectionIdentifier) {
    auto dbMeta = DatabaseMetadata();

    for (const auto &schema : GetSchemas(connectionIdentifier)) {
        dbMeta.AddSchema(schema);

        for (const auto &table :
             GetTablesForSchema(connectionIdentifier, schema)) {
            dbMeta.AddTableForSchema(schema, table);
        }
    }

    return dbMeta;
}

std::vector<std::string> PQGlobal::GetSchemas(int connectionIdentifier) {
    auto ret = std::vector<std::string>();

    auto queryResp = ProcessQuery(connectionIdentifier,
                                  "SELECT schema_name\n"
                                  "FROM information_schema.schemata\n"
                                  "WHERE schema_name !~ '^pg_'\n"
                                  "AND NOT schema_name = 'information_schema'");

    Utils::HandleOptional<void>(
        queryResp, [&ret](const pqxx::result &queryRes) {
            for (pqxx::row row : queryRes) {
                const auto &[schemaName] = row.as<std::string>();

                ret.emplace_back(schemaName);
            }
        });

    return ret;
}

std::vector<std::string> PQGlobal::GetTablesForSchema(int connectionIdentifier,
                                                      std::string_view name) {
    auto ret = std::vector<std::string>();

    auto queryResp = ProcessQuery(
        connectionIdentifier,
        fmt::format("SELECT table_name FROM information_schema.tables\n"
                    "WHERE table_schema = '{}'",
                    name));

    Utils::HandleOptional<void>(
        queryResp, [&ret](const pqxx::result &queryRes) {
            for (pqxx::row row : queryRes) {
                const auto &[schemaName] = row.as<std::string>();

                ret.emplace_back(schemaName);
            }
        });

    return ret;
}

void PQGlobal::RecheckConnectionsOpen() {
    if (!PQGlobal::HasConnectionsOpen())
        return;

    SQD_LOG("Rerunning callbacks for open connections!");
    for (const auto &conn : s_ConnectionsInfoMap) {
        const auto &[id, name] = conn;

        for (const auto &cb : s_SucceedCallbackList) {
            std::invoke(cb, id, name);
        }
    }
}
