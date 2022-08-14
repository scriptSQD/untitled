#include <MainFrame.hpp>

UntitledFrame::UntitledFrame(wxWindow *parent, wxWindowID id,
                             const wxString &title, const wxPoint &pos,
                             const wxSize &size)
    : wxFrame(parent, id, title, pos, size) {
    auto *menu = new wxMenu;
    menu->Append(IdChangeDb, "Change database\tCtrl+D");
    menu->Append(wxID_CLOSE, "Quit\tCtrl+Q");

    auto *menubar = new wxMenuBar;
    menubar->Append(menu, "File");

    this->SetMenuBar(menubar);

    this->sizer = new wxBoxSizer(wxVERTICAL);

    if (PQGlobal::IsConnectionOpen()) {
        this->managementUi = new Tabs(this, wxID_ANY);
        sizer->Add(managementUi, 1, wxEXPAND);
    } else {
        this->startupPanel = new StartupPanel(this, wxID_ANY);
        sizer->Add(startupPanel, 1, wxEXPAND);
    }

    this->SetSizerAndFit(sizer);

    this->Bind(wxEVT_MENU, &UntitledFrame::ChangeDb, IdChangeDb);
    this->Bind(wxEVT_MENU, &UntitledFrame::OnQuit, this, wxID_CLOSE);
    PQGlobal::OnConnected([this] { OnConnected(); });
    PQGlobal::OnConnectionFailed([this] { OnConnectionFailed(); });
}

void UntitledFrame::OnQuit(wxCommandEvent &evt) {
    SQD::Logger::Log("OnQuit: Called close event.");
    this->Close(true);
}
void UntitledFrame::ChangeDb(wxCommandEvent &evt) {
    SQD::Logger::Log("ChangeDb: enter function");
    wxTextEntryDialog dlg(nullptr, "Enter new database connection string:",
                          "Connect to database", "postgresql://localhost");

    if (dlg.ShowModal() == wxID_OK) {
        SQD::Logger::Log("ChangeDb: Got input from modal dialog: " +
                         dlg.GetValue().ToStdString());

        if (!PQGlobal::MakePQConnection(dlg.GetValue().ToStdString()).first)
            wxMessageBox("Connection failed!");

    } else
        SQD::Logger::Log("ChangeDb: Input abandoned.");

    SQD::Logger::Log("ChangeDb: Dialog processed.");
}
void UntitledFrame::OnConnected() {
    SQD::Logger::Log("OnConnected: entering handler.");
    if (startupPanel != nullptr) {
        startupPanel->Show(false);
        ClearSizer();
    }

    if (managementUi == nullptr) {
        managementUi = new Tabs(this, wxID_ANY);
        sizer->Add(managementUi, 1, wxEXPAND);
        sizer->Layout();

        this->Layout();
    }

    if (!managementUi->IsShown())
        managementUi->Show();
}

void UntitledFrame::OnConnectionFailed() {
    if (managementUi != nullptr) {
        managementUi->Show(false);
        ClearSizer();
    }

    if (startupPanel == nullptr) {
        startupPanel = new StartupPanel(this, wxID_ANY);
        sizer->Add(startupPanel, 1, wxEXPAND);
        sizer->Layout();

        this->Layout();
    }

    if (!startupPanel->IsShown())
        startupPanel->Show();
}

void UntitledFrame::ClearSizer() {
    for (int i = 0; i < sizer->GetChildren().size(); i++) {
        sizer->Remove(i);
    }
}
