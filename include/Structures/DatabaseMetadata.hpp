#pragma once

#include <Structures/DatabaseTable.hpp>

#include <optional>
#include <string>
#include <vector>

class DatabaseMetadata {
  public:
    typedef std::tuple<std::string /* Schema name */,
                       std::string /* Table name */>
        TableLocation;

    typedef std::vector<std::tuple<std::string, /* Schema name */
                                   std::string /* Table name */>>
        TablesPerSchemas;

    /**
     * Returns schema name alongside with table name to retrieve it from
     * database later.
     * @param table Table name
     * @return Tuple where first string is schema name and second is table name.
     */
    std::tuple<std::string, std::string>
    GetTableWithSchema(std::string_view table);

    [[nodiscard]] std::vector<std::string> GetSchemas() const;
    void AddSchema(std::string_view name);

    [[nodiscard]] std::string GetCurrentUser() const;

    // TODO: Get tables by schema
    void AddTableForSchema(std::string_view schema, std::string_view table);
    std::vector<std::string> GetTablesForSchema(std::string_view name);

  private:
    std::vector<std::string> m_SchemaList;
    std::string m_CurrentUser;
    TablesPerSchemas m_Tables;
};
