#include "Tabs/TabMain.hpp"

TabMain::TabMain(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                 const wxSize &size)
    : wxWindow(parent, id, pos, size) {

    m_MainSizer = new wxBoxSizer(wxHORIZONTAL);

    m_SchoolListSizer = new wxBoxSizer(wxVERTICAL);
    m_SchoolDetailsSizer = new wxBoxSizer(wxVERTICAL);

    m_Header =
        new wxStaticText(this, wxID_ANY,
                         "Below you will see the list of all schools stored "
                         "in the connected database!");

    m_ReloadButton = new wxButton(this, IdReloadSchoolList, "Reload list.");
    m_ReloadButton->Bind(wxEVT_BUTTON, &TabMain::ReloadSchoolList, this);

    m_schoolListView = new wxListView(this, wxID_ANY);
    //    // For now, this is constant and does not rely on data got from
    //    database. m_schoolListView->AppendColumn("Name");
    //    m_schoolListView->AppendColumn("Phone number");
    //    m_schoolListView->AppendColumn("Address");

    // Fill our UI with data.
    this->PopulateSchoolListCtrl();

    m_SchoolListSizer->Add(m_Header, 0, wxALIGN_LEFT | wxALL, 3);
    m_SchoolListSizer->Add(m_ReloadButton, 0, wxALIGN_LEFT | wxALL, 2);
    m_SchoolListSizer->Add(m_schoolListView, 1, wxEXPAND);
    m_SchoolListSizer->Layout();

    m_MainSizer->Add(m_SchoolListSizer, 2, wxEXPAND | wxALL, 2);
    m_MainSizer->Add(m_SchoolDetailsSizer, 1, wxEXPAND | wxALL, 2);

    this->SetSizerAndFit(m_MainSizer);

    m_schoolListView->Bind(wxEVT_LIST_ITEM_ACTIVATED, [](wxListEvent &evt) {
        SQD_LOG(std::to_string(evt.GetIndex()));
    });
}

void TabMain::RetrieveSchools() {
    m_ColumnsMap = PQGlobal::EnumerateColumns("school_info");

    //    m_SchoolList =
    //        PQGlobal::GetQueryAsData<opt_str_col, opt_str_col, opt_str_col>(
    //            "SELECT name, phone, address FROM "
    //            "sqd.\"sqd.untitled.manager\".school_info");
}

void TabMain::PopulateSchoolListCtrl() {
    this->RetrieveSchools();
    m_schoolListView->ClearAll();
    auto index = 0;

    for (const auto &col : m_ColumnsMap) {
        m_schoolListView->AppendColumn(CapitalizeWord(col.second));
    }

    //    for (const auto &school : m_SchoolList) {
    //
    //        for (const auto &col : m_ColumnsMap) {
    //        auto name = any_cast<std::string>(school);
    //            m_schoolListView->SetItem()
    //        }
    //    }

    //    for (const auto &school : m_SchoolList) {
    //        auto [name, phone, address] = school;
    //
    //        m_schoolListView->InsertItem(index, name.value_or("")); // Insert
    //        name m_schoolListView->SetItem(index, 1,
    //                                  phone.value_or("")); // Insert phone
    //                                  number
    //        m_schoolListView->SetItem(index, 2,
    //                                  address.value_or("")); // Insert address
    //
    //        index++;
    //    }
}

void TabMain::ReloadSchoolList(wxCommandEvent &evt) {
    this->PopulateSchoolListCtrl();
}

void TabMain::OnSchoolSelected(wxListEvent &evt) {}

std::string TabMain::CapitalizeWord(const std::string &source) {
    if (source.length() == 0)
        return "";

    auto ret = source;
    ret[0] = toupper(ret[0]);

    return ret;
}
