#pragma once

#include "UntitledPrecomp.hpp"

#include "Components/DatabaseTreeCtrl.hpp"
#include "Views/TableView.hpp"

#include "wx/aui/auibook.h"
#include "wx/aui/framemanager.h"

struct TableViewClientData {
  public:
    explicit TableViewClientData(int id) : m_Id(id){};

    [[nodiscard]] int GetId() const { return m_Id; }

  private:
    int m_Id;
};

class ManagerView : public wxPanel {
  public:
    explicit ManagerView(wxWindow *parent, wxWindowID winid = wxID_ANY);

    void PopulateDatabaseTree(const std::string &connectionDisplayName,
                              int id) {
        m_DatabaseTreeCtrl->AppendToTree(connectionDisplayName, id);
    }

  private:
    void ForgetTableView(int id);

    void OnTreeItemSelected(wxTreeEvent &evt);
    void ResolveTableView(const DatabaseMetadata::TableLocation &location);

  private:
    DatabaseTreeCtrl *m_DatabaseTreeCtrl;
    wxAuiPaneInfo *m_DatabaseTreePaneInfo;

    DatabaseMetadata m_DatabaseMetadata;
    std::unordered_map<DatabaseMetadata::TableLocation, int,
                       HashDatabaseTableLocation>
        m_TableViews;

    wxAuiManager *m_AuiMgr;
    wxAuiNotebook *m_ViewTabs;
};