#include <MainFrame.hpp>

UntitledFrame::UntitledFrame(wxWindow *parent, wxWindowID id,
                             const wxString &title, const wxPoint &pos,
                             const wxSize &size)
    : wxFrame(parent, id, title, pos, size) {
    auto *fileMenu = new wxMenu();
    fileMenu->Append(IdChangeDb, "New connection\tCtrl+N");
    fileMenu->Append(wxID_CLOSE, "Quit\tCtrl+Q");

    auto *toggleViewsMenu = new wxMenu();
    toggleViewsMenu->AppendCheckItem(IdToggleDatabaseTree, "Database tree",
                                     "If not toggled, database tree will be "
                                     "hidden from application viewport.");

    auto *viewMenu = new wxMenu();
    viewMenu->AppendSubMenu(toggleViewsMenu, "Toggle views");

    auto *menubar = new wxMenuBar;
    menubar->Append(fileMenu, "File");
    menubar->Append(viewMenu, "View");

    this->SetMenuBar(menubar);

    this->sizer = new wxBoxSizer(wxVERTICAL);

    if (PQGlobal::IsConnectionOpen()) {
        this->managementUi = new Tabs(this, wxID_ANY);
        sizer->Add(managementUi, 1, wxEXPAND);
        this->FitInside();
    } else {
        this->startupPanel = new StartupPanelView(this, wxID_ANY);
        sizer->Add(startupPanel, 1, wxEXPAND | wxALL, 3);
        this->FitInside();
    }

    this->SetSizerAndFit(sizer);

    this->Bind(wxEVT_MENU, &UntitledFrame::ChangeDb, IdChangeDb);
    this->Bind(wxEVT_MENU, &UntitledFrame::OnQuit, this, wxID_CLOSE);
    PQGlobal::OnConnected([this] { OnConnected(); });
    PQGlobal::OnConnectionFailed([this] { OnConnectionFailed(); });
}

void UntitledFrame::OnQuit(wxCommandEvent &evt) {
    SQD_LOG("OnQuit: Called close event.");
    this->Close(true);
}

void UntitledFrame::ChangeDb(wxCommandEvent &evt) {
    SQD_LOG("ChangeDb: enter function");
    wxTextEntryDialog dlg(nullptr, "Enter new database connection string:",
                          "Connect to database", "postgresql://localhost");

    if (dlg.ShowModal() == wxID_OK) {
        SQD_LOG("ChangeDb: Got input from modal dialog: " +
                dlg.GetValue().ToStdString());

        if (!PQGlobal::MakePQConnection(dlg.GetValue().ToStdString()).first)
            wxMessageBox("Connection failed!");

    } else
        SQD_LOG("ChangeDb: Input abandoned.");

    SQD_LOG("ChangeDb: Dialog processed.");
}

void UntitledFrame::OnConnected() {
    SQD_LOG("OnConnected: entering handler.");
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
        startupPanel = new StartupPanelView(this, wxID_ANY);
        sizer->Add(startupPanel, 1, wxEXPAND | wxALL, 3);
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
