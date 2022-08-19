#include <Components/DatabaseConnectionWizard.hpp>

DatabaseConnectionWizard::DatabaseConnectionWizard(wxWindow *parent)
    : wxPanel(parent) {
    m_Sizer = new wxGridSizer(1, wxSize(4, 4));

    m_CredentialsSizer = new wxBoxSizer(wxHORIZONTAL);
    m_AddressSizer = new wxBoxSizer(wxHORIZONTAL);
    m_MiscSizer = new wxBoxSizer(wxHORIZONTAL);

    m_ConnectionName = new LabeledTextCtrl(
        this, wxID_ANY, "Connection name:", wxEmptyString, wxSize(450, -1));

    m_Hostname = new LabeledTextCtrl(this, wxID_ANY, "Hostname:", "localhost",
                                     wxSize(370, -1));
    m_Port =
        new LabeledTextCtrl(this, wxID_ANY, "Port:", "5432", wxSize(80, -1));

    m_AddressSizer->Add(m_Hostname, 0, wxALIGN_LEFT | wxALL, 1);
    m_AddressSizer->Add(m_Port, 0, wxALIGN_LEFT | wxALL, 1);

    m_Username = new LabeledTextCtrl(this, wxID_ANY, "Username:", wxEmptyString,
                                     wxSize(225, -1));
    m_Password = new LabeledTextCtrl(this, wxID_ANY, "Password:", wxEmptyString,
                                     wxSize(225, -1), wxTE_PASSWORD);
    m_CredentialsSizer->Add(m_Username, 0, wxALIGN_LEFT | wxALL, 1);
    m_CredentialsSizer->Add(m_Password, 0, wxALIGN_LEFT | wxALL, 1);

    m_Database = new LabeledTextCtrl(
        this, wxID_ANY, "Database name:", wxEmptyString, wxSize(225, -1));
    m_Params = new LabeledTextCtrl(this, wxID_ANY, "Params:", wxEmptyString,
                                   wxSize(225, -1));
    m_MiscSizer->Add(m_Database, 0, wxALIGN_LEFT | wxALL, 1);
    m_MiscSizer->Add(m_Params, 0, wxALIGN_LEFT | wxALL, 1);

    // Append everything to sizer.
    m_Sizer->Add(m_ConnectionName, 0, wxEXPAND | wxALL, 2);

    m_Sizer->Add(m_AddressSizer, 0, wxEXPAND | wxALL, 2);
    m_Sizer->Add(m_CredentialsSizer, 0, wxEXPAND | wxALL, 2);
    m_Sizer->Add(m_MiscSizer, 0, wxEXPAND | wxALL, 2);

    this->SetSizerAndFit(m_Sizer);
}

std::string DatabaseConnectionWizard::GetConnectionString() const {
    // Start with the protocol
    constexpr std::string_view protocol = "postgresql";

    // Connection destination
    const auto &hostname = m_Hostname->GetInput().ToStdString();
    const auto &port = m_Port->GetInput().ToStdString();

    const auto &address = hostname.empty() ? ""
                          : port.empty()   ? fmt::format("{}", hostname)
                                         : fmt::format("{}:{}", hostname, port);

    // Credentials
    const auto &username = m_Username->GetInput().ToStdString();
    const auto &password = m_Password->GetInput().ToStdString();

    const auto &credentials = username.empty() ? ""
                              : password.empty()
                                  ? fmt::format("{}", username)
                                  : fmt::format("{}:{}", username, password);

    // Misc
    const auto &database = m_Database->GetInput().ToStdString();
    const auto &params = m_Params->GetInput().ToStdString();

    const auto &misc = params.empty() ? fmt::format("/{}", database)
                                      : fmt::format("/{}?{}", database, params);

    return credentials.empty()
               ? fmt::format("{}://{}{}", protocol, address, misc)
               : fmt::format("{}://{}@{}{}", protocol, credentials, address,
                             misc);
}

void DatabaseConnectionWizard::ClearInput() {
    m_ConnectionName->ResetInput();

    m_Hostname->ResetInput();
    m_Port->ResetInput();

    m_Username->ResetInput();
    m_Password->ResetInput();

    m_Database->ResetInput();
    m_Params->ResetInput();
}
