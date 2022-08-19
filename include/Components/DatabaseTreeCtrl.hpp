#pragma once

#include <UntitledPrecomp.hpp>

#include "wx/treebase.h"
#include "wx/treectrl.h"

#include <Structures/DatabaseMetadata.hpp>

class DatabaseTreeCtrl : public wxPanel {
  public:
    DatabaseTreeCtrl(wxWindow *parent);

  private:
    void IntrospectDatabase();
    void PopulateDatabaseTree();

  private:
    wxBoxSizer *m_Sizer;

    wxTreeCtrl *m_DatabaseTreeView;
    DatabaseMetadata m_DatabaseMeta;
};