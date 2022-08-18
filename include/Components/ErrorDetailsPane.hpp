#pragma once

#include "UntitledPrecomp.hpp"

#include "wx/collpane.h"

class ErrorDetailsPane : public wxCollapsiblePane {
  public:
    ErrorDetailsPane(wxWindow *parent, wxWindowID id, const wxString &label,
                     std::string error_details);

    void UpdateDetailsText(const std::string &error_details);

  private:
    wxWindow *m_PaneContents;

    std::string m_Details;
    wxBoxSizer *m_Sizer;
    wxStaticText *m_DetailsText;
};