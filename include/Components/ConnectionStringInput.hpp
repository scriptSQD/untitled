#pragma once

#include <UntitledPrecomp.hpp>

#include "Components/LabeledTextCtrl.hpp"
#include "Interfaces/ConnectionProviderInteface.hpp"

class ConnectionStringInput : public wxPanel, public IConnectionStringProvider {
  public:
    explicit ConnectionStringInput(wxWindow *parent);

    [[nodiscard]] std::string GetConnectionString() const override;

    void ClearInput() override { m_Input->ResetInput(); };

  private:
    wxBoxSizer *m_Sizer;

    LabeledTextCtrl *m_Input;
};
