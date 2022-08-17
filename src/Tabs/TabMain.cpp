#include "Tabs/TabMain.hpp"

TabMain::TabMain(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                 const wxSize &size)
    : wxPanel(parent, id, pos, size) {

    m_MainSizer = new wxBoxSizer(wxHORIZONTAL);

    m_SchoolListSizer = new wxBoxSizer(wxVERTICAL);
    m_SchoolDetailsSizer = new wxBoxSizer(wxVERTICAL);

    m_Header =
        new wxStaticText(this, wxID_ANY,
                         "Below you will see the list of all schools stored "
                         "in the connected database!");

    m_ReloadButton = new wxButton(this, IdReloadSchoolList, "Reload list.");
    m_ReloadButton->SetToolTip("Queries database for fresh data.");

    m_ReloadButton->Bind(wxEVT_BUTTON, &TabMain::ReloadSchoolList, this);

    m_schoolListView = new wxListView(this, wxID_ANY);

    // Fill our UI with data.
    this->PopulateSchoolListCtrl();

    m_SchoolListSizer->Add(m_Header, 0, wxALIGN_LEFT | wxALL, 3);
    m_SchoolListSizer->Add(m_ReloadButton, 0, wxALIGN_LEFT | wxALL, 2);
    m_SchoolListSizer->Add(m_schoolListView, 1, wxEXPAND | wxTOP, 2);
    m_SchoolListSizer->Layout();

    m_MainSizer->Add(m_SchoolListSizer, 2, wxEXPAND | wxALL, 2);
    m_MainSizer->Add(m_SchoolDetailsSizer, 1, wxEXPAND | wxALL, 2);

    this->SetSizerAndFit(m_MainSizer);

    m_schoolListView->Bind(wxEVT_LIST_ITEM_ACTIVATED, [](wxListEvent &evt) {
        SQD_LOG(std::to_string(evt.GetIndex()));
    });
}

void TabMain::RetrieveSchools() {
    m_SchoolList = PQGlobal::ParseTable("school_info");
}

void TabMain::PopulateSchoolListCtrl() {
    this->RetrieveSchools();
    m_schoolListView->ClearAll();

    for (const auto &col : m_SchoolList.GetColumns()) {
        m_schoolListView->AppendColumn(CapitalizeString(col));
    }

    auto index = 0;
    for (const auto &row : m_SchoolList.GetRows()) {
        m_schoolListView->InsertItem(index, "");

        auto colIndex = 0;
        for (const auto &colVal : row) {
            m_schoolListView->SetItem(index, colIndex, wxString(colVal).wc_str());
            colIndex++;
        }
        index++;
    }
}

void TabMain::ReloadSchoolList(wxCommandEvent &evt) {
    this->PopulateSchoolListCtrl();
}

void TabMain::OnSchoolSelected(wxListEvent &evt) {}

std::string TabMain::CapitalizeString(const std::string &source) {
    if (source.length() == 0)
        return "";

    auto ret = source;
    ret[0] = (char)toupper(ret[0]);

    return ret;
}
