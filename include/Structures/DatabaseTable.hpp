#pragma once

// Pretty much optional
#include <Logger.hpp>

#include <optional>
#include <string>
#include <vector>

struct DatabaseTable {
  public:
    typedef std::vector<std::string> ColumnList;

    /**
     * Map containing column name as a key and column value a a value.
     */
    typedef std::pair<std::string, std::string> ColumnData;

    /**
     * Stores multiple column mappings for one row.
     */
    typedef std::vector<ColumnData> RowData;

    /**
     * Stores multiple row containers from a table.
     */
    typedef std::vector<RowData> TableData;

    std::optional<std::vector<std::string>>
    GetColumnValues(std::string_view colName);
    std::optional<std::vector<std::string>> GetRow(int rowNumber);

    ColumnList GetColumns() { return m_ColMetadata; }

    size_t GetColumnCount() { return m_ColMetadata.size(); }
    size_t GetRowCount() { return m_Data.size(); }

    std::vector<std::vector<std::string>> GetRows();

    void InsertColumn(std::string_view name) {
        m_ColMetadata.emplace_back(name);
    }

    void InsertRow(const RowData &row) { m_Data.emplace_back(row); }

  private:
    ColumnList m_ColMetadata;
    TableData m_Data;
};