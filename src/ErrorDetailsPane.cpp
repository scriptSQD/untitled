#include <ErrorDetailsPane.hpp>
#include <utility>

ErrorDetailsPane::ErrorDetailsPane(wxWindow *parent, wxWindowID id,
                                   const wxString &label,
                                   std::string error_details)
    : wxCollapsiblePane(parent, id, label),
      m_Details(std::move(error_details)) {

    this->SetBackgroundColour(wxTransparentColor);
    this->SetFont(UntitledFonts::FONT_REGULAR);

    this->m_PaneContents = this->GetPane();
    this->m_Sizer = new wxBoxSizer(wxVERTICAL);

    m_PaneContents->SetBackgroundColour(wxTransparentColor);

    this->m_DetailsText =
        new wxStaticText(m_PaneContents, wxID_ANY, this->m_Details);

    m_DetailsText->SetFont(UntitledFonts::FONT_REGULAR);
    m_DetailsText->SetForegroundColour("#cf000f");

    m_Sizer->Add(m_DetailsText, 0, wxGROW | wxALL, 2);

    m_PaneContents->SetSizerAndFit(m_Sizer);
    m_Sizer->SetSizeHints(m_PaneContents);

    m_DetailsText->Wrap(m_PaneContents->GetSize().GetWidth());

    m_Sizer->Layout();
    m_PaneContents->Layout();
}
void ErrorDetailsPane::UpdateDetailsText(const std::string &error_details) {
    this->m_Details = error_details;

    this->m_DetailsText->SetLabel(this->m_Details);

    m_Sizer->Layout();
    m_PaneContents->Layout();
    this->Layout();
}
