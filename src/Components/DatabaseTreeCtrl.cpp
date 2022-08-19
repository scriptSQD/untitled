#include <Components/DatabaseTreeCtrl.hpp>

DatabaseTreeCtrl::DatabaseTreeCtrl(wxWindow *parent)
    : wxScrolledWindow(parent), m_IconsList(nullptr) {
    m_Sizer = new wxBoxSizer(wxVERTICAL);

    PopulateIcons();

    m_DatabaseTreeView = new wxTreeCtrl(
        this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_FULL_ROW_HIGHLIGHT);
    m_DatabaseTreeView->SetImageList(m_IconsList);
    m_DatabaseTreeView->SetFocus();

    PopulateDatabaseTree();

    ConstructQuickActionsPanel();

    m_Sizer->Add(m_QuickActionsPanel, 0, wxEXPAND | wxALL, 2);
    m_Sizer->Add(m_DatabaseTreeView, 1, wxEXPAND | wxTOP, 2);

    this->SetSizerAndFit(m_Sizer);
}

void DatabaseTreeCtrl::IntrospectDatabase() {
    m_DatabaseMeta = PQGlobal::IntrospectDatabase();
}

void DatabaseTreeCtrl::PopulateDatabaseTree() {
    m_DatabaseTreeView->DeleteAllItems();

    IntrospectDatabase();

    const auto &rootId = m_DatabaseTreeView->AddRoot("Database root node");
    for (const auto &schema : m_DatabaseMeta.GetSchemas()) {
        const auto &schemaNodeId =
            m_DatabaseTreeView->AppendItem(rootId, schema, 1);

        for (const auto &table : m_DatabaseMeta.GetTablesForSchema(schema)) {
            m_DatabaseTreeView->AppendItem(schemaNodeId, table, 2, -1,
                                           new TableNodeData({schema, table}));
        }
    }
}

void DatabaseTreeCtrl::PopulateIcons() {
    auto dimens = this->FromDIP(wxSize(22, 22));
    m_IconsList =
        new wxImageList(dimens.GetWidth(), dimens.GetHeight(), true, 3);

    for (auto icon :
         {"resources/database-hidpi.png", "resources/schema-hidpi.png",
          "resources/table-hidpi.png"}) {
        auto *bmp = new wxBitmap(icon);
        wxBitmap::Rescale(*bmp, dimens);
        m_IconsList->Add(*bmp);
    }
}

void DatabaseTreeCtrl::ConstructQuickActionsPanel() {
    const auto &addBmpBundle =
        Utils::CreateBitmapBundle("resources/add", "png");
    const auto &deleteBmpBundle =
        Utils::CreateBitmapBundle("resources/delete", "png");
    const auto &syncBmpBundle =
        Utils::CreateBitmapBundle("resources/sync", "png");

    m_Add = new wxBitmapButton(this, IdDatabaseTreeAdd, addBmpBundle);
    m_Add->Bind(wxEVT_BUTTON, &DatabaseTreeCtrl::OnQuickAdd, this);

    m_Delete = new wxBitmapButton(this, IdDatabaseTreeDelete, deleteBmpBundle);
    m_Delete->Bind(wxEVT_BUTTON, &DatabaseTreeCtrl::OnQuickDelete, this);

    m_Sync = new wxBitmapButton(this, IdDatabaseTreeSync, syncBmpBundle);
    m_Sync->Bind(wxEVT_BUTTON, &DatabaseTreeCtrl::OnQuickSync, this);

    m_QuickActionsPanel = new QAPanel(this, {m_Add, m_Delete, m_Sync});
}

void DatabaseTreeCtrl::OnQuickAdd(wxCommandEvent &evt) {
    SQD_LOG("Caught Quick.Add event!");
}

void DatabaseTreeCtrl::OnQuickDelete(wxCommandEvent &evt) {
    SQD_LOG("Caught Quick.Delete event!");
}

void DatabaseTreeCtrl::OnQuickSync(wxCommandEvent &evt) {
    SQD_LOG("Caught Quick.Sync event!");
    RefreshTree();
}
