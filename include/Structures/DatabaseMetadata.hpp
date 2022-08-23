#pragma once

#include <Structures/DatabaseTable.hpp>

#include <optional>
#include <string>
#include <vector>

class DatabaseMetadata {
  public:
    typedef std::tuple<int /* Connection identifier */,
                       std::string /* Schema name */,
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
    [[nodiscard]] std::tuple<std::string, std::string>
    GetTableWithSchema(std::string_view table) const;

    [[nodiscard]] std::vector<std::string> GetSchemas() const;
    void AddSchema(std::string_view name);

    // TODO: Get tables by schema
    void AddTableForSchema(std::string_view schema, std::string_view table);
    std::vector<std::string> GetTablesForSchema(std::string_view name);

    [[nodiscard]] int GetConnectionId() const { return m_ConnectionId; }

  private:
    std::vector<std::string> m_SchemaList;
    int m_ConnectionId;
    TablesPerSchemas m_Tables;
};
