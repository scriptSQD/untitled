#pragma once

#include "UntitledPrecomp.hpp"

class LabeledTextCtrl : public wxPanel {
  public:
    /**
     * A simple wrapper around wxPanel that creates wxStaticText as well as
     * wxTextCtrl to look like a monolithic input field.
     */
    LabeledTextCtrl(wxWindow *parent, wxWindowID id, const wxString &label,
                    const wxSize &ctrlSize = wxDefaultSize, const int &gap = 3);

    void SetDirection(const wxOrientation &orient);
    void ResetInput();
    wxString GetInput();

  private:
    wxBoxSizer *sizer;

    wxStaticText *labelText;
    wxTextCtrl *ctrl;
};