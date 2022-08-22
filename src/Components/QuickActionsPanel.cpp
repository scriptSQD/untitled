#include <Components/QuickActionsPanel.hpp>
#include <utility>

QAPanel::QAPanel(wxWindow *parent, PanelControls buttons)
    : wxScrolledWindow(parent),
      m_ButtonsWithCallbacks(std::move(buttons)) {
    m_Sizer = new wxBoxSizer(wxHORIZONTAL);

    for (auto button : m_ButtonsWithCallbacks) {
        button->Reparent(this);
        m_Sizer->Add(button, 0, wxALIGN_CENTER | wxALL, 2);
    }

    this->SetSizerAndFit(m_Sizer);
}
