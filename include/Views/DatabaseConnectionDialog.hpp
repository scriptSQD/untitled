#pragma once

#include <UntitledPrecomp.hpp>

#include "Components/ErrorDetailsMessageDialog.hpp"
#include "Views/DatabaseConnectionView.hpp"

#include "wx/button.h"

class DatabaseConnectionDialog : public wxDialog {
  public:
    DatabaseConnectionDialog();

  private:
    void OnConnectButton(wxCommandEvent &evt);
    void OnCancelButton(wxCommandEvent &evt);

  private:
    wxDialog *m_Dialog;
    DatabaseConnectionView *m_DatabaseConnectionView;

    wxBoxSizer *m_MainSizer, *m_ButtonSizer;
    wxButton *m_ButtonConnect, *m_ButtonCancel;
};
