#include "Views/StartupPanelView.hpp"

StartupPanelView::StartupPanelView(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id),
      m_Sizer(new wxStaticBoxSizer(wxVERTICAL, this, "Quick start")) {
    this->m_Header = new wxStaticText(
        this, wxID_ANY, "You are not connected to any database!",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);

    this->m_SubHeader =
        new wxStaticText(this, wxID_ANY,
                         "This is required to continue. Please "
                         "fill out required information below:",
                         wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);

    m_ConnectionView = new DatabaseConnectionView(this);

    m_ConnectButton = new wxButton(this, IdConnect, "Connect");

    // *most likely* temporary solution for error details
    this->m_ErrorDetailsButton = new wxCommandLinkButton(
        this, IdErrorDetails, "Error details:", "Get error details.");
    m_ErrorDetailsButton->Disable();
    m_ErrorDetailsButton->Hide();

    m_Header->SetFont(UntitledFonts::FONT_H1);
    m_Header->Wrap(m_Header->GetCharWidth() * 28);
    m_SubHeader->SetFont(UntitledFonts::FONT_H2);
    m_SubHeader->Wrap(m_SubHeader->GetCharWidth() * 32);

    m_Sizer->Add(m_Header, 0, wxALIGN_CENTER | wxALL, 4);
    m_Sizer->Add(m_SubHeader, 0, wxALIGN_CENTER | wxALL, 4);

    m_Sizer->Add(m_ConnectionView, 0, wxALIGN_CENTER | wxALL | wxSHRINK, 4);

    m_Sizer->Add(m_ConnectButton, 0, wxALIGN_CENTER | wxALL, 4);
    m_Sizer->Add(m_ErrorDetailsButton, 0,
                 wxALIGN_CENTER | wxALL | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 4);

    this->SetSizerAndFit(m_Sizer);

    m_ConnectButton->Bind(wxEVT_BUTTON, &StartupPanelView::OnConnectButton,
                          this, IdConnect);

    m_ErrorDetailsButton->Bind(
        wxEVT_BUTTON,
        [this](wxCommandEvent &evt) {
            wxMessageBox(m_ErrorDetails, "Error details");
        },
        IdErrorDetails);
}

void StartupPanelView::OnConnectButton(wxCommandEvent &evt) {
    SQD_LOG("OnConnectButton: Enter handler.");

    const auto &connStr =
        m_ConnectionView->GetProvider()->GetConnectionString();

    auto [success, errorDetails] = PQGlobal::MakePQConnection(connStr);

    if (!success) {
        wxMessageBox(
            "Connection failed!\nCheck out the details on button below.",
            "Failure.");
        m_ErrorDetails = errorDetails;
        m_ErrorDetailsButton->Show();
        m_ErrorDetailsButton->Enable();

        return;
    }

    m_ConnectionView->ClearInput();
    m_ErrorDetailsButton->Hide();
    m_ErrorDetailsButton->Disable();
    m_ErrorDetails = "";
}
