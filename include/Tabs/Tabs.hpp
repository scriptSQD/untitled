#pragma once

#include <UntitledPrecomp.hpp>

#include "Components/DatabaseTreeCtrl.hpp"
#include "Views/TableView.hpp"

#include "wx/aui/auibook.h"
#include "wx/aui/framemanager.h"


class Tabs : public wxPanel {
  public:
    explicit Tabs(wxWindow *parent, wxWindowID winid = wxID_ANY);

  private:
    void OnTreeItemSelected(wxTreeEvent &evt);

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