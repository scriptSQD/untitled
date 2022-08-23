#pragma once

#include <UntitledPrecomp.hpp>

#include "Components/LabeledTextCtrl.hpp"
#include "Interfaces/ConnectionProviderInteface.hpp"

class ConnectionStringInput : public wxPanel, public ConnectionInfoProvider {
  public:
    explicit ConnectionStringInput(wxWindow *parent);

    [[nodiscard]] std::string GetConnectionString() const override;
    [[nodiscard]] std::string GetConnectionDisplayName() const override;

    void ClearInput() override { m_ConnStringInput->ResetInput(); };

  private:
    wxBoxSizer *m_Sizer;

    LabeledTextCtrl *m_ConnDisplayNameInput, *m_ConnStringInput;
};
