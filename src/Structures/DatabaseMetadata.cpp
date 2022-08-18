#include <Structures/DatabaseMetadata.hpp>

std::vector<std::string> DatabaseMetadata::GetSchemas() const {
    return m_SchemaList;
}

void DatabaseMetadata::AddSchema(std::string_view name) {
    m_SchemaList.emplace_back(name);
}
std::string DatabaseMetadata::GetCurrentUser() const { return m_CurrentUser; }

std::tuple<std::string, std::string>
DatabaseMetadata::GetTableWithSchema(std::string_view table) {
    for (const auto &schema : m_Tables) {
        const auto [schemaName, tableName] = schema;
        if (tableName == table)
            return {schemaName, tableName};
    }

    return {};
}
void DatabaseMetadata::AddTableForSchema(std::string_view schema,
                                         std::string_view table) {
    m_Tables.emplace_back(std::string(schema), std::string(table));
}

std::vector<std::string>
DatabaseMetadata::GetTablesForSchema(std::string_view name) {
    auto ret = std::vector<std::string>();

    for (const auto &tableData : m_Tables) {
        const auto &[schemaName, tableName] = tableData;

        if (schemaName == name)
            ret.emplace_back(tableName);
    }

    return ret;
}
