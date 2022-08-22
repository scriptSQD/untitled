#include <MainFrame.hpp>

UntitledFrame::UntitledFrame(wxWindow *parent, wxWindowID id,
                             const wxString &title, const wxPoint &pos,
                             const wxSize &size)
    : wxFrame(parent, id, title, pos, size) {
    auto *fileMenu = new wxMenu();
    fileMenu->Append(IdAddConnection, "New connection\tCtrl+N");
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

    this->m_Sizer = new wxBoxSizer(wxVERTICAL);

    this->m_ManagerView = new ManagerView(this, wxID_ANY);
    m_Sizer->Add(m_ManagerView, 1, wxEXPAND);

    // If connection was initialized on app startup, process callbacks
    // that are set by ManagerView
    PQGlobal::RecheckConnectionsOpen();

    this->SetSizerAndFit(m_Sizer);

    this->Bind(wxEVT_MENU, &UntitledFrame::AddConnection, IdAddConnection);
    this->Bind(wxEVT_MENU, &UntitledFrame::OnQuit, this, wxID_CLOSE);
}

void UntitledFrame::OnQuit(wxCommandEvent &evt) {
    SQD_LOG("OnQuit: Called close event.");
    this->Close(true);
}

void UntitledFrame::AddConnection(wxCommandEvent &evt) {
    SQD_LOG("AddConnection: enter handler.");

    new DatabaseConnectionDialog();
}
