#include <StartupPanel.hpp>

StartupPanel::StartupPanel(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id), sizer(new wxBoxSizer(wxVERTICAL)) {

    this->m_Header = new wxStaticText(
        this, wxID_ANY, "You are not connected to any database!",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    this->m_SubHeader =
        new wxStaticText(this, wxID_ANY,
                         "This is required to continue. Please "
                         "fill out required information below:",
                         wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);

    this->m_Input = new LabeledTextCtrl(this, wxID_ANY,
                                        "Connection string:", wxSize(500, 30));

    this->m_ConnectButton = new wxButton(this, IdConnect, "Connect!");

    // *most likely* temporary solution for error details
    this->m_ErrorDetailsButton = new wxCommandLinkButton(
        this, IdErrorDetails,
        "Error details:", "Click here to get additional error details.");
    m_ErrorDetailsButton->Disable();

    m_Header->SetFont(UntitledFonts::FONT_H1);
    m_Header->Wrap(500);
    m_SubHeader->SetFont(UntitledFonts::FONT_H2);
    m_SubHeader->Wrap(550);

    sizer->Add(m_Header, 0, wxALIGN_CENTER | wxALL, 4);
    sizer->Add(m_SubHeader, 0, wxALIGN_CENTER | wxALL, 4);
    sizer->Add(m_Input, 0, wxALIGN_CENTER | wxALL, 4);
    sizer->Add(m_ConnectButton, 0, wxALIGN_CENTER | wxALL, 4);
    sizer->Add(m_ErrorDetailsButton, 0, wxALIGN_CENTER | wxALL, 4);

    sizer->Layout();
    this->SetSizerAndFit(sizer);

    m_ConnectButton->Bind(wxEVT_BUTTON, &StartupPanel::OnConnectButton, this,
                          IdConnect);
    m_ErrorDetailsButton->Bind(
        wxEVT_BUTTON,
        [this](wxCommandEvent &evt) { wxMessageBox(m_ErrorDetails); },
        IdErrorDetails);
}

void StartupPanel::OnConnectButton(wxCommandEvent &evt) {
    SQD::Logger::Log("OnConnectButton: Enter handler.");
    if (m_Input->GetInput().empty()) {
        SQD::Logger::Log("OnConnectButton: User provided no connection string",
                         SQD::Logger::LEVEL_WARNING);
        wxMessageBox("Please provide non-empty string to the input box!");
        return;
    }

    auto [success, errorDetails] =
        PQGlobal::MakePQConnection(m_Input->GetInput().ToStdString());

    if (!success) {
        wxMessageBox(
            "Connection failed!\nCheck out the details via button below.");
        m_ErrorDetails = errorDetails;
        m_ErrorDetailsButton->Enable();

        return;
    }

    m_Input->ResetInput();
    m_ErrorDetailsButton->Disable();
    m_ErrorDetails = "";
}
