#pragma once

#include <UntitledPrecomp.hpp>

#include "ManagerView.hpp"
#include "Views/DatabaseConnectionDialog.hpp"
#include "Views/StartupPanelView.hpp"

class UntitledFrame : public wxFrame {

  public:
    UntitledFrame(wxWindow *parent, wxWindowID id, const wxString &title,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize);

  private:
    static void AddConnection(wxCommandEvent &evt);

    void OnQuit(wxCommandEvent &evt);

  private:
    ManagerView *m_ManagerView;
    wxBoxSizer *m_Sizer;
};