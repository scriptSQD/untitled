#pragma once

#include "UntitledPrecomp.hpp"

#include "Views/DatabaseConnectionView.hpp"

#include "wx/commandlinkbutton.h"

class StartupPanelView : public wxPanel {
  public:
    StartupPanelView(wxWindow *parent, wxWindowID id);

  private:
    wxStaticBoxSizer *m_Sizer;

    DatabaseConnectionView *m_ConnectionView;

    wxStaticText *m_Header, *m_SubHeader;
    wxButton *m_ConnectButton;
    wxCommandLinkButton *m_ErrorDetailsButton;

    std::string m_ErrorDetails;

    void OnConnectButton(wxCommandEvent &evt);
};
