#pragma once

#include <UntitledPrecomp.hpp>
#include <optional>
#include <any>

#include <wx/listctrl.h>

using opt_str_col = std::optional<std::string>;
using opt_int_col = std::optional<int>;

using School =
    std::tuple<opt_str_col, opt_str_col, opt_str_col>;

class TabMain : public wxWindow {
  public:
    TabMain(wxWindow *parent, wxWindowID id,
            const wxPoint &pos = wxDefaultPosition,
            const wxSize &size = wxDefaultSize);

  private:
    void RetrieveSchools();
    void PopulateSchoolListCtrl();
    void ReloadSchoolList(wxCommandEvent &evt);
    void OnSchoolSelected(wxListEvent &evt);

  private:
    std::vector<std::any> m_SchoolList;

    wxStaticText *m_Header;
    wxBoxSizer *m_MainSizer, *m_SchoolListSizer, *m_SchoolDetailsSizer;
    wxListView *m_schoolListView;
    wxButton *m_ReloadButton;

    std::string CapitalizeWord(const std::string &source);

    // We can make use of maps and map
    // and map columns to their indexes, that way we list all data present in db
    // as well as we can dynamically create wxListView columns and pass names for them
    // from map keys/values.
    std::map<int, std::string> m_ColumnsMap;
};