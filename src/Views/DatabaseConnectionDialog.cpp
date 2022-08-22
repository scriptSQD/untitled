#include "Views/DatabaseConnectionDialog.hpp"

DatabaseConnectionDialog::DatabaseConnectionDialog() {
    SQD_LOG("DatabaseConnectionDialog: enter dialog constructor.");

    m_MainSizer = new wxBoxSizer(wxVERTICAL);
    m_ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    m_Dialog = new wxDialog(nullptr, wxID_ANY, "Add new database connection",
                            wxDefaultPosition, wxDefaultSize,
                            wxCAPTION | wxRESIZE_BORDER | wxSTAY_ON_TOP);

    // TODO: Add a "Test connection" button here

    m_ButtonConnect = new wxButton(m_Dialog, wxID_ANY, "OK");
    m_ButtonCancel = new wxButton(m_Dialog, wxID_EXIT, "Cancel");

    m_DatabaseConnectionView = new DatabaseConnectionView(m_Dialog);

    m_ButtonSizer->Add(m_ButtonConnect, 0, wxALIGN_CENTER | wxALL, 3);
    m_ButtonSizer->Add(m_ButtonCancel, 0, wxALIGN_CENTER | wxALL, 3);

    m_MainSizer->Add(m_DatabaseConnectionView, 1, wxEXPAND | wxALL, 3);
    m_MainSizer->Add(m_ButtonSizer, 0, wxALIGN_RIGHT | wxALL, 3);

    m_ButtonConnect->Bind(wxEVT_BUTTON,
                          &DatabaseConnectionDialog::OnConnectButton, this);
    m_ButtonCancel->Bind(wxEVT_BUTTON,
                         &DatabaseConnectionDialog::OnCancelButton, this);

    m_Dialog->SetSizerAndFit(m_MainSizer);

    // Open modal
    m_Dialog->ShowModal();

    m_Dialog->Destroy();
}

void DatabaseConnectionDialog::OnConnectButton(wxCommandEvent &evt) {
    const auto &connStr = m_DatabaseConnectionView->GetConnectionString();
    const auto &connName = m_DatabaseConnectionView->GetConnectionDisplayName();

    const auto &[id, error] = PQGlobal::AddConnection(connStr, connName);

    if (id == -1) {
        new ErrorDetailsMessageDialog(
            "Connection failed!",
            "Connection failed with the following details", error);
    } else {
        // Should be at the very end
        m_Dialog->EndModal(wxID_OK);
    }
}

void DatabaseConnectionDialog::OnCancelButton(wxCommandEvent &evt) {
    // Should be at the very end
    m_Dialog->EndModal(wxID_CANCEL);
}
