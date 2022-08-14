#include <LabeledTextCtrl.hpp>

LabeledTextCtrl::LabeledTextCtrl(wxWindow *parent, wxWindowID id,
                                 const wxString &label, const wxSize &ctrlSize,
                                 const int &gap)
    : wxPanel(parent, id) {
    this->sizer = new wxBoxSizer(wxVERTICAL);

    this->labelText = new wxStaticText(this, wxID_ANY, label);
    labelText->SetFont(UntitledFonts::FONT_REGULAR);

    this->ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, ctrlSize);

    sizer->Add(labelText, 0, wxALIGN_LEFT | wxALL, gap);
    sizer->Add(ctrl, 0, wxALIGN_LEFT | wxALL, gap);

    this->SetSizerAndFit(sizer);
}
void LabeledTextCtrl::SetDirection(const wxOrientation &orient) {
    sizer->SetOrientation(orient);
    sizer->Layout();
}
wxString LabeledTextCtrl::GetInput() { return ctrl->GetValue(); }
void LabeledTextCtrl::ResetInput() { ctrl->SetValue(""); }
