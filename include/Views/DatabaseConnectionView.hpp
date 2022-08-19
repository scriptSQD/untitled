#pragma once

#include "UntitledPrecomp.hpp"

#include "Components/ConnectionStringInput.hpp"
#include "Components/DatabaseConnectionWizard.hpp"
#include "Interfaces/ConnectionProviderInteface.hpp"

class DatabaseConnectionView : public wxPanel {
  public:
    explicit DatabaseConnectionView(wxWindow *parent);

    IConnectionStringProvider *GetProvider() { return m_CurrentProvider; }

    void ClearInput() { m_CurrentProvider->ClearInput(); }

  private:
    wxBoxSizer *m_Sizer, *m_RadioSizer;

    wxRadioButton *m_WizardRadio, *m_ConnStrRadio;

    DatabaseConnectionWizard *m_Wizard;
    ConnectionStringInput *m_ConnStringInput;

    IConnectionStringProvider *m_CurrentProvider;
};
