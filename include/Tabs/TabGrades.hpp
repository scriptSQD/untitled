#pragma once

#include <UntitledPrecomp.hpp>

class TabGrades : public wxPanel {
  public:
    TabGrades(wxWindow *parent, wxWindowID id, const wxPoint &pos,
              const wxSize &size);

  private:
    wxBoxSizer *m_Sizer;
};