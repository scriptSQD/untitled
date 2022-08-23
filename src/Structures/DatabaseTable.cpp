#include <Structures/DatabaseTable.hpp>

std::optional<std::vector<std::string>>
DatabaseTable::GetColumnValues(std::string_view colName) {
    if (std::find(m_ColMetadata.begin(), m_ColMetadata.end(), colName) ==
        m_ColMetadata.end()) {
        SQD_WARN("GetColumnValues: Requested column '" + std::string(colName) +
                 " that does not exist.");
        return {};
    }

    auto ret = std::vector<std::string>();

    const auto &it =
        std::find(m_ColMetadata.begin(), m_ColMetadata.end(), colName);

    if (it == m_ColMetadata.end()) {
        SQD_WARN("GetColumnValues: No such column found!");
        return {};
    }

    const size_t valueOffset = it - m_ColMetadata.begin();

    for (const auto &row : m_Data) {
        const auto &[cn, value] = row.at(valueOffset);
        ret.emplace_back(value);
    }

    return ret;
}

std::optional<std::vector<std::string>> DatabaseTable::GetRow(int rowNumber) {
    if (rowNumber >= m_Data.size()) {
        SQD_WARN("GetRow: Requested row number '" + std::to_string(rowNumber) +
                 "' that does not exist.");
        return {};
    }

    auto ret = std::vector<std::string>();

    for (const auto &col : m_Data.at(rowNumber)) {
        const auto &[cn, value] = col;
        ret.emplace_back(value);
    }

    return ret;
}

std::vector<std::vector<std::string>> DatabaseTable::GetRows() {
    if (m_Data.empty())
        return {};

    auto ret = std::vector<std::vector<std::string>>();
    for (int i = 0; i < GetRowCount(); i++) {
        const auto &row = GetRow(i);
        if (!row.has_value()) {
            break;
        }

        ret.emplace_back(row.value());
    }

    return ret;
}
