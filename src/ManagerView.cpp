#include "ManagerView.hpp"

ManagerView::ManagerView(wxWindow *parent, wxWindowID winid)
    : wxPanel(parent, winid) {
    m_AuiMgr =
        new wxAuiManager(this, wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE);

    m_DatabaseTreeCtrl = new DatabaseTreeCtrl(this);

    m_DatabaseTreePaneInfo = new wxAuiPaneInfo();
    m_DatabaseTreePaneInfo->MinSize(250, -1);
    m_DatabaseTreePaneInfo->Left();
    m_DatabaseTreePaneInfo->Caption("Database tree");

    m_ViewTabs =
        new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    m_AuiMgr->AddPane(m_DatabaseTreeCtrl, *m_DatabaseTreePaneInfo);
    m_AuiMgr->AddPane(m_ViewTabs, wxCENTER, "View tabs");

    m_AuiMgr->Update();

    PQGlobal::OnConnected([this](int id, const std::string &displayName) {
        m_DatabaseTreeCtrl->AppendToTree(displayName, id);
    });

    PQGlobal::OnConnectionClosed([this](int identifier) {
        m_DatabaseTreeCtrl->RemoveConnection(identifier);
    });

    m_DatabaseTreeCtrl->BindTreeEvt(
        wxEVT_TREE_ITEM_ACTIVATED,
        [this](wxTreeEvent &evt) { OnTreeItemSelected(evt); });

    TableView::OnDestroy([this](int winid) { ForgetTableView(winid); });
}

void ManagerView::OnTreeItemSelected(wxTreeEvent &evt) {
    auto tree = m_DatabaseTreeCtrl->GetTreeCtrl();

    const auto &data =
        dynamic_cast<TableNodeData *>(tree->GetItemData(evt.GetItem()));

    if (data == nullptr) {
        evt.Skip();
        return;
    }

    if (!data->IsTableNode()) {
        evt.Skip();
        return;
    }

    const auto &location = data->GetLocation();

    ResolveTableView(location);
}

void ManagerView::ResolveTableView(
    const DatabaseMetadata::TableLocation &location) {
    try {
        auto pageId = m_TableViews.at(location);

        dynamic_cast<TableView *>(FindWindowById(pageId))->UpdateTable();

        m_ViewTabs->SetSelection(
            m_ViewTabs->GetPageIndex(FindWindowById(pageId)));
    } catch (...) {
        const auto &id = int(m_TableViews.size());
        m_TableViews[location] = id;

        const auto &[connId, schema, table] = location;
        const auto &connName = PQGlobal::GetConnectionName(connId);

        auto *tableView = new TableView(this, id, location);
        tableView->SetClientData(new TableViewClientData(id));

        m_ViewTabs->AddPage(tableView,
                            fmt::format("{}:{} [{}]", schema, table, connName));

        m_ViewTabs->SetSelection(m_ViewTabs->GetPageIndex(FindWindowById(id)));
    }
}

void ManagerView::ForgetTableView(int id) {
    const auto &it = std::find_if(m_TableViews.begin(), m_TableViews.end(),
                                  [&id](auto &view) {
                                      const auto &[location, tableId] = view;
                                      return tableId == id;
                                  });

    if (it != m_TableViews.end()) {
        m_TableViews.erase(it);
    }
}
