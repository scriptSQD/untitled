#pragma once

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Tabs/TabMain.hpp"

#include "wx/aui/auibook.h"
#include "wx/aui/framemanager.h"

#include "wx/treectrl.h"
#include "wx/treebase.h"

#include <Structures/DatabaseMetadata.hpp>
#include <Structures/DatabaseTable.hpp>

class Tabs : public wxPanel {
  public:
    explicit Tabs(wxWindow *parent, wxWindowID winid = wxID_ANY);

  private:
    void IntrospectDatabase();
    void PopulateDatabaseTree();

  private:
    DatabaseMetadata m_DatabaseMetadata;
    std::unordered_map<DatabaseMetadata::TableLocation, int,
                       HashDatabaseTableLocation>
        m_TableViews;

    wxAuiManager *m_AuiMgr;
    wxTreeCtrl *m_DatabaseTree;
    wxAuiNotebook *m_ViewTabs;
};