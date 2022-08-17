#pragma once

#include <UntitledPrecomp.hpp>
#include <any>
#include <optional>

#include <wx/listctrl.h>

class TabMain : public wxPanel {
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
    DatabaseTable m_SchoolList;

    wxStaticText *m_Header;
    wxBoxSizer *m_MainSizer, *m_SchoolListSizer, *m_SchoolDetailsSizer;
    wxListView *m_schoolListView;
    wxButton *m_ReloadButton;

    static std::string CapitalizeString(const std::string &source);
};