#pragma once

#include <UntitledPrecomp.hpp>

#include <StartupPanel.hpp>
#include <Tabs/Tabs.hpp>

class UntitledFrame : public wxFrame {

  public:
    UntitledFrame(wxWindow *parent, wxWindowID id, const wxString &title,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize);

  private:
    StartupPanel *startupPanel = nullptr;
    Tabs *managementUi = nullptr;
    wxBoxSizer *sizer;

    void ClearSizer();
    void OnConnected();
    void OnConnectionFailed();
    static void ChangeDb(wxCommandEvent &evt);
    void OnQuit(wxCommandEvent &evt);
};