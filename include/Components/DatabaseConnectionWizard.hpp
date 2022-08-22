#pragma once

#include <UntitledPrecomp.hpp>

#include "Interfaces/ConnectionProviderInteface.hpp"
#include <Components/LabeledTextCtrl.hpp>

class DatabaseConnectionWizard : public wxPanel, public ConnectionInfoProvider {
  public:
    explicit DatabaseConnectionWizard(wxWindow *parent);

    [[nodiscard]] std::string GetConnectionString() const override;
    [[nodiscard]] std::string GetConnectionDisplayName() const override;

    void ClearInput() override;

  private:
    wxGridSizer *m_Sizer;
    wxBoxSizer *m_CredentialsSizer, *m_AddressSizer, *m_MiscSizer;

    LabeledTextCtrl *m_Hostname, *m_Port, *m_Username, *m_Password, *m_Database,
        *m_Params, *m_ConnectionName;
};