#pragma once

#include "UntitledPrecomp.hpp"

#include "ErrorDetailsPane.hpp"
#include "LabeledTextCtrl.hpp"
#include "wx/commandlinkbutton.h"

class StartupPanel : public wxPanel {
  public:
    StartupPanel(wxWindow *parent, wxWindowID id);

  private:
    wxStaticBoxSizer *sizer;

    wxStaticText *m_Header, *m_SubHeader;
    wxButton *m_ConnectButton;
    wxCommandLinkButton *m_ErrorDetailsButton;
    LabeledTextCtrl *m_Input;

    std::string m_ErrorDetails;

    void OnConnectButton(wxCommandEvent &evt);
};
