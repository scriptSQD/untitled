#include "Components/ConnectionStringInput.hpp"

ConnectionStringInput::ConnectionStringInput(wxWindow *parent)
    : wxPanel(parent) {
    m_Sizer = new wxBoxSizer(wxVERTICAL);

    m_Input = new LabeledTextCtrl(this, wxID_ANY, "Connection string:",
                                  "postgresql://localhost", wxSize(450, -1));

    m_Sizer->Add(m_Input, 0, wxALIGN_CENTER | wxALL, 2);

    this->SetSizerAndFit(m_Sizer);
}
std::string ConnectionStringInput::GetConnectionString() const {
    return m_Input->GetInput().ToStdString();
}
