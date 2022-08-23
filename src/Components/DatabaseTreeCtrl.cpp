#include <Components/DatabaseTreeCtrl.hpp>

DatabaseTreeCtrl::DatabaseTreeCtrl(wxWindow *parent)
    : wxScrolledWindow(parent), m_IconsList(nullptr) {
    m_Sizer = new wxBoxSizer(wxVERTICAL);

    PopulateIcons();

    m_DatabaseTreeView = new wxTreeCtrl(
        this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_FULL_ROW_HIGHLIGHT);
    m_RootNodeId = m_DatabaseTreeView->AddRoot("Database tree root node");

    m_DatabaseTreeView->SetImageList(m_IconsList);

    ConstructQuickActionsPanel();

    m_Sizer->Add(m_QuickActionsPanel, 0, wxEXPAND | wxALL, 2);
    m_Sizer->Add(m_DatabaseTreeView, 1, wxEXPAND | wxTOP, 2);

    this->SetSizerAndFit(m_Sizer);
}

void DatabaseTreeCtrl::IntrospectDatabase(int connectionIdentifier) {
    m_DatabaseMeta = PQGlobal::IntrospectDatabase(connectionIdentifier);
}

void DatabaseTreeCtrl::RemoveConnection(int identifier) {
    const auto &connectionNode = std::pair<int, std::string>(
        identifier, PQGlobal::GetConnectionName(identifier));

    const auto &it = std::find(m_ActiveConnectionNodes.begin(),
                               m_ActiveConnectionNodes.end(), connectionNode);

    if (it == m_ActiveConnectionNodes.end()) {
        return;
    }

    m_ActiveConnectionNodes.erase(it);
    RefreshTree();
}

void DatabaseTreeCtrl::RefreshTree() {
    m_DatabaseTreeView->DeleteChildren(m_RootNodeId);

    for (const auto &connection : m_ActiveConnectionNodes) {
        const auto &[id, displayName] = connection;

        IntrospectDatabase(id);

        const auto &connectionRootNode =
            m_DatabaseTreeView->AppendItem(m_RootNodeId, displayName, 0, -1,
                                           new TableNodeData(id, true, false));

        for (const auto &schema : m_DatabaseMeta.GetSchemas()) {
            const auto &schemaNodeId = m_DatabaseTreeView->AppendItem(
                connectionRootNode, schema, 2, -1, new TableNodeData(id));

            for (const auto &table :
                 m_DatabaseMeta.GetTablesForSchema(schema)) {
                m_DatabaseTreeView->AppendItem(
                    schemaNodeId, table, 3, -1,
                    new TableNodeData({id, schema, table}));
            }
        }
    }
}

void DatabaseTreeCtrl::PopulateDatabaseTree(
    int connectionIdentifier, const std::string &connectionDisplayName) {
    // Record the connection
    m_ActiveConnectionNodes.emplace_back(connectionIdentifier,
                                         connectionDisplayName);

    IntrospectDatabase(connectionIdentifier);

    const auto &connectionRootNode = m_DatabaseTreeView->AppendItem(
        m_RootNodeId, connectionDisplayName, 0, -1,
        new TableNodeData(connectionIdentifier, true, false));

    for (const auto &schema : m_DatabaseMeta.GetSchemas()) {
        const auto &schemaNodeId = m_DatabaseTreeView->AppendItem(
            connectionRootNode, schema, 2, -1,
            new TableNodeData(connectionIdentifier));

        for (const auto &table : m_DatabaseMeta.GetTablesForSchema(schema)) {
            m_DatabaseTreeView->AppendItem(
                schemaNodeId, table, 3, -1,
                new TableNodeData({connectionIdentifier, schema, table}));
        }
    }
}

void DatabaseTreeCtrl::PopulateIcons() {
    auto dimens = this->FromDIP(wxSize(22, 22));
    m_IconsList =
        new wxImageList(dimens.GetWidth(), dimens.GetHeight(), true, 3);

    std::string connectionBmpPath;
    std::string databaseBmpPath;
    std::string schemaBmpPath;
    std::string tableBmpPath;

    wxSystemAppearance systemAppearance = wxSystemSettings::GetAppearance();
    if (systemAppearance.IsUsingDarkBackground()) {
        connectionBmpPath = "resources/assets/dark/connection-hidpi.png";
        databaseBmpPath = "resources/assets/dark/database-hidpi.png";
        schemaBmpPath = "resources/assets/dark/schema-hidpi.png";
        tableBmpPath = "resources/assets/dark/table-hidpi.png";
    } else {
        connectionBmpPath = "resources/assets/light/connection-hidpi.png";
        databaseBmpPath = "resources/assets/light/database-hidpi.png";
        schemaBmpPath = "resources/assets/light/schema-hidpi.png";
        tableBmpPath = "resources/assets/light/table-hidpi.png";
    }

    for (const auto &icon :
         {connectionBmpPath, databaseBmpPath, schemaBmpPath, tableBmpPath}) {
        m_IconsList->Add(Utils::CreateBitmap(icon, wxBITMAP_TYPE_PNG, dimens));
    }
}

void DatabaseTreeCtrl::ConstructQuickActionsPanel() {
    auto dimens = this->FromDIP(wxSize(22, 22));

    std::string addBmpPath;
    std::string deleteBmpPath;
    std::string syncBmpPath;

    wxSystemAppearance systemAppearance = wxSystemSettings::GetAppearance();
    if (systemAppearance.IsUsingDarkBackground()) {
        addBmpPath = "resources/assets/dark/add";
        deleteBmpPath = "resources/assets/dark/delete_forever";
        syncBmpPath = "resources/assets/dark/sync";
    } else {
        addBmpPath = "resources/assets/light/add";
        deleteBmpPath = "resources/assets/light/delete_forever";
        syncBmpPath = "resources/assets/light/sync";
    }

    const auto &addBmpBundle =
        Utils::CreateBitmapBundle(addBmpPath, "png", wxBITMAP_TYPE_PNG, dimens);
    const auto &delBmpBundle = Utils::CreateBitmapBundle(
        deleteBmpPath, "png", wxBITMAP_TYPE_PNG, dimens);
    const auto &syncBmpBundle = Utils::CreateBitmapBundle(
        syncBmpPath, "png", wxBITMAP_TYPE_PNG, dimens);

    m_Add = new wxBitmapButton(this, IdDatabaseTreeAdd, addBmpBundle);
    m_Add->Bind(wxEVT_BUTTON, &DatabaseTreeCtrl::OnQuickAdd);

    m_Delete = new wxBitmapButton(this, IdDatabaseTreeDelete, delBmpBundle);
    m_Delete->Bind(wxEVT_BUTTON, &DatabaseTreeCtrl::OnQuickDelete, this);

    m_Sync = new wxBitmapButton(this, IdDatabaseTreeSync, syncBmpBundle);
    m_Sync->Bind(wxEVT_BUTTON, &DatabaseTreeCtrl::OnQuickSync, this);

    m_QuickActionsPanel = new QAPanel(this, {m_Add, m_Delete, m_Sync});
}

void DatabaseTreeCtrl::OnQuickAdd(wxCommandEvent &evt) {
    SQD_LOG("Caught Quick.Add event!");
    new DatabaseConnectionDialog();
}

void DatabaseTreeCtrl::OnQuickDelete(wxCommandEvent &evt) {
    SQD_LOG("Caught Quick.Delete event!");
    // TODO: Close connection on this event via PQGlobal API.

    const auto &selectedData = dynamic_cast<TableNodeData *>(
        m_DatabaseTreeView->GetItemData(m_DatabaseTreeView->GetSelection()));

    if (!selectedData) {
        evt.Skip();
        return;
    }

    const auto &id = selectedData->GetConnectionIdentifier();
    const auto &connName = PQGlobal::GetConnectionName(id);

    wxMessageDialog dlg(nullptr,
                        fmt::format("The following action will close "
                                    "connection '{}'.\nAre you sure?",
                                    connName),
                        "Close connection?", wxYES_NO | wxCENTER);

    if (dlg.ShowModal() == wxID_YES) {
        SQD_LOG("Closing connection");
        PQGlobal::CloseConnection(selectedData->GetConnectionIdentifier());
    }
}

void DatabaseTreeCtrl::OnQuickSync(wxCommandEvent &evt) {
    SQD_LOG("Caught Quick.Sync event!");
    RefreshTree();
}
