#include "Views/DatabaseConnectionView.hpp"

DatabaseConnectionView::DatabaseConnectionView(wxWindow *parent)
    : wxPanel(parent) {
    m_Sizer = new wxBoxSizer(wxVERTICAL);

    m_RadioSizer = new wxBoxSizer(wxHORIZONTAL);

    m_WizardRadio = new wxRadioButton(this, IdProviderWizard, "Wizard");
    m_ConnStrRadio =
        new wxRadioButton(this, IdProviderRaw, "Connection string");

    m_RadioSizer->Add(m_WizardRadio, 0, wxALIGN_CENTER | wxALL, 3);
    m_RadioSizer->Add(m_ConnStrRadio, 0, wxALIGN_CENTER | wxALL, 3);

    m_Wizard = new DatabaseConnectionWizard(this);
    m_CurrentProvider = m_Wizard;

    m_ConnStringInput = new ConnectionStringInput(this);
    m_ConnStringInput->Hide();

    m_Sizer->Add(m_RadioSizer, 0, wxALIGN_CENTER | wxALL, 4);

    m_Sizer->Add(m_Wizard, 0, wxALIGN_CENTER | wxALL, 4);
    m_Sizer->Add(m_ConnStringInput, 0, wxALIGN_CENTER | wxALL, 4);

    this->SetSizerAndFit(m_Sizer);

    this->Bind(wxEVT_RADIOBUTTON, [this](wxCommandEvent &evt) {
        if (evt.GetId() == IdProviderWizard) {
            m_CurrentProvider = m_Wizard;
        } else if (evt.GetId() == IdProviderRaw) {
            m_CurrentProvider = m_ConnStringInput;
        }

        m_Wizard->Show(evt.GetId() == IdProviderWizard);
        m_ConnStringInput->Show(evt.GetId() == IdProviderRaw);
    });
}
