#include "Tabs/TabMain.hpp"

#include <utility>

TabMain::TabMain(wxWindow *parent, wxWindowID winid,
                 DatabaseMetadata::TableLocation table)
    : wxPanel(parent, winid), m_Table(std::move(table)) {

    m_MainSizer = new wxBoxSizer(wxHORIZONTAL);

    m_ListSizer = new wxBoxSizer(wxVERTICAL);
    m_FieldDetailsSizer = new wxBoxSizer(wxVERTICAL);

    m_Header = new wxStaticText(
        this, wxID_ANY,
        "Below you will see the data stored in one of the database tables!");

    m_ReloadButton = new wxButton(this, IdReloadSchoolList, "Reload list.");
    m_ReloadButton->SetToolTip("Query database for fresh data");

    m_ReloadButton->Bind(wxEVT_BUTTON, &TabMain::ReloadItemList, this);

    m_ListView = new wxListView(this, wxID_ANY);

    // Fill our UI with data.
    this->PopulateItemListCtrl();

    m_ListSizer->Add(m_Header, 0, wxALIGN_LEFT | wxALL, 3);
    m_ListSizer->Add(m_ReloadButton, 0, wxALIGN_LEFT | wxALL, 2);
    m_ListSizer->Add(m_ListView, 1, wxEXPAND | wxTOP, 3);
    m_ListSizer->Layout();

    m_MainSizer->Add(m_ListSizer, 2, wxEXPAND | wxALL, 2);
    m_MainSizer->Add(m_FieldDetailsSizer, 1, wxEXPAND | wxALL, 2);

    this->SetSizerAndFit(m_MainSizer);

    m_ListView->Bind(wxEVT_LIST_ITEM_ACTIVATED, [](wxListEvent &evt) {
        SQD_LOG(std::to_string(evt.GetIndex()));
    });
}

void TabMain::RetrieveTableData() {
    const auto &[schema, table] = m_Table;
    m_DataList = PQGlobal::ParseTable(schema, table);
}

void TabMain::PopulateItemListCtrl() {
    this->RetrieveTableData();
    m_ListView->ClearAll();

    for (const auto &col : m_DataList.GetColumns()) {
        m_ListView->AppendColumn(CapitalizeString(col));
    }

    auto index = 0;
    for (const auto &row : m_DataList.GetRows()) {
        m_ListView->InsertItem(index, "");

        auto colIndex = 0;
        for (const auto &colVal : row) {
            m_ListView->SetItem(index, colIndex, wxString(colVal).wc_str());
            colIndex++;
        }
        index++;
    }
}

void TabMain::ReloadItemList(wxCommandEvent &evt) {
    this->PopulateItemListCtrl();
}

void TabMain::OnItemSelected(wxListEvent &evt) {}

std::string TabMain::CapitalizeString(const std::string &source) {
    if (source.length() == 0)
        return "";

    std::string ret = source;
    ret[0] = (char)toupper(ret[0]);

    return ret;
}
