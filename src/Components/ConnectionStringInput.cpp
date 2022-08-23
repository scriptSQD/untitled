#include "Components/ConnectionStringInput.hpp"

ConnectionStringInput::ConnectionStringInput(wxWindow *parent)
    : wxPanel(parent) {
    m_Sizer = new wxBoxSizer(wxVERTICAL);

    m_ConnDisplayNameInput = new LabeledTextCtrl(
        this, wxID_ANY, "Display name:",
        fmt::format("New connection {}", PQGlobal::GetLastConnectionId()),
        wxSize(400, -1));

    m_ConnStringInput = new LabeledTextCtrl(
        this, wxID_ANY, "Connection string:", "postgresql://localhost",
        wxSize(450, -1));

    m_Sizer->Add(m_ConnDisplayNameInput, 0, wxALIGN_LEFT | wxALL, 2);
    m_Sizer->Add(m_ConnStringInput, 0, wxALIGN_LEFT | wxALL, 2);

    this->SetSizerAndFit(m_Sizer);
}
std::string ConnectionStringInput::GetConnectionString() const {
    return m_ConnStringInput->GetInput().ToStdString();
}
std::string ConnectionStringInput::GetConnectionDisplayName() const {
    if (auto n = m_ConnDisplayNameInput->GetInput().ToStdString(); !n.empty())
        return n;

    return fmt::format("Untitled connection {}",
                       PQGlobal::GetLastConnectionId());
}
