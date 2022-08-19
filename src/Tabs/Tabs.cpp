#include "Tabs/Tabs.hpp"

Tabs::Tabs(wxWindow *parent, wxWindowID winid) : wxPanel(parent, winid) {
    m_AuiMgr =
        new wxAuiManager(this, wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE);

    m_DatabaseTreeCtrl = new DatabaseTreeCtrl(this);

    m_DatabaseTreePaneInfo = new wxAuiPaneInfo();
    m_DatabaseTreePaneInfo->BestSize(wxSize(300, -1));
    m_DatabaseTreePaneInfo->Left();
    m_DatabaseTreePaneInfo->Caption("Database tree");

    m_ViewTabs =
        new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    m_AuiMgr->AddPane(m_DatabaseTreeCtrl, *m_DatabaseTreePaneInfo);
    m_AuiMgr->AddPane(m_ViewTabs, wxCENTER, "View tabs");

    m_AuiMgr->Update();

    PQGlobal::OnConnected([this]() { m_DatabaseTreeCtrl->RefreshTree(); });
    m_DatabaseTreeCtrl->BindTreeEvt(
        wxEVT_TREE_ITEM_ACTIVATED,
        [this](wxTreeEvent &evt) { OnTreeItemSelected(evt); });

    //    m_ViewTabs->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, [](wxAuiNotebookEvent
    //    &evt) {
    //        SQD_LOG(fmt::format("AuiNotebook Page Close with id: {}",
    //        evt.GetId())); evt.Veto();
    //    });
}

void Tabs::OnTreeItemSelected(wxTreeEvent &evt) {
    auto tree = m_DatabaseTreeCtrl->GetTreeCtrl();

    const auto &data = (TableNodeData *)tree->GetItemData(evt.GetItem());

    if (data == nullptr) {
        evt.Skip();
        return;
    }

    const auto &location = data->GetLocation();

    try {
        auto pageId = m_TableViews.at(location);

        ((TableView *)(FindWindowById(pageId)))->UpdateTable();

        m_ViewTabs->SetSelection(
            m_ViewTabs->GetPageIndex(FindWindowById(pageId)));
    } catch (...) {
        const auto &id = int(m_TableViews.size());
        m_TableViews[location] = id;

        m_ViewTabs->AddPage(
            new TableView(this, id, location),
            fmt::format("{}:{}", std::get<0>(location), std::get<1>(location)));

        m_ViewTabs->SetSelection(m_ViewTabs->GetPageIndex(FindWindowById(id)));
    }
}
