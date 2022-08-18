#include "Tabs/Tabs.hpp"

Tabs::Tabs(wxWindow *parent, wxWindowID winid) : wxPanel(parent, winid) {
    m_AuiMgr =
        new wxAuiManager(this, wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE);

    m_DatabaseTree = new wxTreeCtrl(this);
    PopulateDatabaseTree();

    m_ViewTabs = new wxAuiNotebook(this);

    m_AuiMgr->AddPane(m_DatabaseTree, wxLEFT, "Database tree");
    m_AuiMgr->GetPane(m_DatabaseTree).BestSize(wxSize(250, -1));
    m_AuiMgr->AddPane(m_ViewTabs, wxCENTER, "View tabs");

    m_AuiMgr->Update();

    m_DatabaseTree->Bind(wxEVT_TREE_ITEM_ACTIVATED, [this](wxTreeEvent &evt) {
        auto parent = m_DatabaseTree->GetItemParent(evt.GetItem());
        if (!parent.IsOk()) {
            evt.Skip();
            return;
        }

        const auto &schema = m_DatabaseTree->GetItemText(parent).ToStdString();
        const auto &table =
            m_DatabaseTree->GetItemText(evt.GetItem()).ToStdString();

        try {
            auto pageId = m_TableViews.at({schema, table});
            SQD_LOG(fmt::format("m_TableViews has id {} opened.", pageId));

            ((TabMain *)(FindWindowById(pageId)))->UpdateTable();

            m_ViewTabs->SetSelection(
                m_ViewTabs->GetPageIndex(FindWindowById(pageId)));
        } catch (...) {
            const auto &id = int(m_TableViews.size());
            m_TableViews[{schema, table}] = id;
            SQD_LOG(fmt::format("m_TableViews added id {} to its opened list.",
                                id));

            m_ViewTabs->AddPage(new TabMain(this, id, {schema, table}),
                                fmt::format("{}:{}", schema, table));

            m_ViewTabs->SetSelection(
                m_ViewTabs->GetPageIndex(FindWindowById(id)));
        }
    });

    m_ViewTabs->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, [](wxAuiNotebookEvent &evt) {
        SQD_LOG(fmt::format("AuiNotebook Page Close with id: {}", evt.GetId()));
        evt.Veto();
    });
}

void Tabs::IntrospectDatabase() {
    m_DatabaseMetadata = PQGlobal::IntrospectDatabase();
}

void Tabs::PopulateDatabaseTree() {
    IntrospectDatabase();

    for (const auto &schema : m_DatabaseMetadata.GetSchemas()) {
        auto rootId = m_DatabaseTree->AddRoot(schema);
        m_DatabaseTree->SetToolTip("Some tooltip");

        for (const auto &table :
             m_DatabaseMetadata.GetTablesForSchema(schema)) {
            m_DatabaseTree->AppendItem(rootId, table);
        }
    }
}
