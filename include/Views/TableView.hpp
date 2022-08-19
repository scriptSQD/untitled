#pragma once

#include "UntitledPrecomp.hpp"
#include <any>
#include <optional>

#include "wx/listctrl.h"

class TableView : public wxPanel {
  public:
    TableView(wxWindow *parent, wxWindowID winid, DatabaseMetadata::TableLocation table);
    void UpdateTable() { PopulateItemListCtrl(); };

  private:
    void RetrieveTableData();
    void PopulateItemListCtrl();
    void ReloadItemList(wxCommandEvent &evt);
    void OnItemSelected(wxListEvent &evt);

  private:
    static std::string CapitalizeString(const std::string &source);

  private:
    DatabaseMetadata::TableLocation m_Table;

    DatabaseTable m_DataList;

    wxStaticText *m_Header;
    wxBoxSizer *m_MainSizer, *m_ListSizer, *m_FieldDetailsSizer;
    wxListView *m_ListView;
    wxButton *m_ReloadButton;
};