#include <Components/DatabaseTreeCtrl.hpp>

DatabaseTreeCtrl::DatabaseTreeCtrl(wxWindow *parent) : wxPanel(parent) {
    m_Sizer = new wxBoxSizer(wxVERTICAL);

    m_DatabaseTreeView = new wxTreeCtrl(
        this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_FULL_ROW_HIGHLIGHT);
}

void DatabaseTreeCtrl::IntrospectDatabase() {
    m_DatabaseMeta = PQGlobal::IntrospectDatabase();
}

void DatabaseTreeCtrl::PopulateDatabaseTree() {
    IntrospectDatabase();

    const auto &rootId = m_DatabaseTreeView->AddRoot("*Database name*");
    for (const auto &schema : m_DatabaseMeta.GetSchemas()) {
        const auto &schemaNodeId =
            m_DatabaseTreeView->AppendItem(rootId, schema);

        for (const auto &table : m_DatabaseMeta.GetTablesForSchema(schema)) {
            m_DatabaseTreeView->AppendItem(schemaNodeId, table);
        }
    }
}
