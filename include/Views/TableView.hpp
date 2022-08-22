#pragma once

#include "UntitledPrecomp.hpp"
#include <any>
#include <optional>

#include "wx/listctrl.h"

class TableView : public wxPanel {
  public:
    TableView(wxWindow *parent, wxWindowID winid,
              DatabaseMetadata::TableLocation table);
    ~TableView() override {
        SQD_LOG("Destroying TableView");
        for (const auto &cb : m_OnDestroyCallbacks) {
            std::invoke(cb, this->GetId());
        }
    }
    void UpdateTable() { PopulateItemListCtrl(); };

    static void OnDestroy(const std::function<void(int winid)> &callback) {
        m_OnDestroyCallbacks.emplace_back(callback);
    }

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

    inline static std::vector<std::function<void(int winid)>>
        m_OnDestroyCallbacks;
};