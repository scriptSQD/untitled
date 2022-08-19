#pragma once

#include <UntitledPrecomp.hpp>

#include <wx/scrolwin.h>

class QAPanel : public wxScrolledWindow {
  public:
    typedef std::vector<wxControl*> PanelControls;

  public:
    QAPanel(wxWindow *parent, PanelControls buttonsWithCallbacks);

  private:
    wxBoxSizer *m_Sizer;

    PanelControls m_ButtonsWithCallbacks;
};