#pragma once

#include "UntitledPrecomp.hpp"

#include "Components/ConnectionStringInput.hpp"
#include "Components/DatabaseConnectionWizard.hpp"
#include "Interfaces/ConnectionProviderInteface.hpp"

class DatabaseConnectionView : public wxPanel {
  public:
    explicit DatabaseConnectionView(wxWindow *parent);

    ConnectionInfoProvider *GetProvider() { return m_CurrentProvider; }

    std::string GetConnectionString() {
        return GetProvider()->GetConnectionString();
    }
    std::string GetConnectionDisplayName() {
        return GetProvider()->GetConnectionDisplayName();
    }

    void ClearInput() { m_CurrentProvider->ClearInput(); }

  private:
    wxBoxSizer *m_Sizer, *m_RadioSizer;

    wxRadioButton *m_WizardRadio, *m_ConnStrRadio;

    DatabaseConnectionWizard *m_Wizard;
    ConnectionStringInput *m_ConnStringInput;

    ConnectionInfoProvider *m_CurrentProvider;
};
