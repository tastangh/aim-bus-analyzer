// fileName: ui/ModeSelectionDialog.cpp
#include "ModeSelectionDialog.hpp"
#include <wx/statline.h>

ModeSelectionDialog::ModeSelectionDialog(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxArrayString mainModeOptions;
    mainModeOptions.Add("Single-Mode Operation");
    mainModeOptions.Add("Custom Multi-Mode Operation");
    mainModeOptions.Add("Scenario: Concurrent BC+RT+BM");
    m_mainModeChooser = new wxRadioBox(this, wxID_ANY, "Select Operation Type", wxDefaultPosition, wxDefaultSize, mainModeOptions, 1, wxRA_SPECIFY_COLS);
    mainSizer->Add(m_mainModeChooser, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    wxArrayString singleModeOptions;
    singleModeOptions.Add("Bus Controller (BC)");
    singleModeOptions.Add("Remote Terminal (RT)");
    singleModeOptions.Add("Bus Monitor (BM)");
    m_radioBoxSingleChoice = new wxRadioBox(this, wxID_ANY, "Select a single mode", wxDefaultPosition, wxDefaultSize, singleModeOptions, 1, wxRA_SPECIFY_COLS);
    m_singleModeSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Single-Mode Options");
    m_singleModeSizer->Add(m_radioBoxSingleChoice, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(m_singleModeSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    m_multiModeSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Custom Multi-Mode Options");
    m_checkBC = new wxCheckBox(this, wxID_ANY, "Bus Controller (BC)");
    m_checkRT = new wxCheckBox(this, wxID_ANY, "Remote Terminal (RT)");
    m_checkBM = new wxCheckBox(this, wxID_ANY, "Bus Monitor (BM)");
    m_multiModeSizer->Add(m_checkBC, 0, wxALL, 5);
    m_multiModeSizer->Add(m_checkRT, 0, wxALL, 5);
    m_multiModeSizer->Add(m_checkBM, 0, wxALL, 5);
    mainSizer->Add(m_multiModeSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    m_scenarioModeSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Scenario Description");
    wxStaticText* scenarioText = new wxStaticText(this, wxID_ANY, "Runs a predefined scenario where a BC, multiple RTs, and a BM operate concurrently on the same device, similar to the 'ls_bcrtbm' sample.");
    scenarioText->Wrap(350);
    m_scenarioModeSizer->Add(scenarioText, 0, wxALL, 10);
    mainSizer->Add(m_scenarioModeSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);

    m_mainModeChooser->Bind(wxEVT_RADIOBOX, &ModeSelectionDialog::OnMainModeChange, this);
    Bind(wxEVT_BUTTON, &ModeSelectionDialog::OnOK, this, wxID_OK);
    
    m_mainModeChooser->SetSelection(0);
    UpdateControlStates();

    SetSizerAndFit(mainSizer);
    Centre();
}

void ModeSelectionDialog::OnMainModeChange(wxCommandEvent& event) {
    UpdateControlStates();
}

void ModeSelectionDialog::OnOK(wxCommandEvent& event) {
    int mainSelection = m_mainModeChooser->GetSelection();
    m_config.mainMode = static_cast<OperationMode>(mainSelection);

    // Başlangıçta tüm seçimleri temizle
    m_config.bc_selected = false;
    m_config.rt_selected = false;
    m_config.bm_selected = false;
    m_config.singleModeSelection = -1;

    if (m_config.mainMode == OperationMode::SINGLE) {
        m_config.singleModeSelection = m_radioBoxSingleChoice->GetSelection();
        if (m_config.singleModeSelection == 0) m_config.bc_selected = true;
        if (m_config.singleModeSelection == 1) m_config.rt_selected = true;
        if (m_config.singleModeSelection == 2) m_config.bm_selected = true;
    } else if (m_config.mainMode == OperationMode::CUSTOM_MULTI) {
        m_config.bc_selected = m_checkBC->IsChecked();
        m_config.rt_selected = m_checkRT->IsChecked();
        m_config.bm_selected = m_checkBM->IsChecked();
    } else if (m_config.mainMode == OperationMode::SCENARIO_BC_RT_BM) {
        // DÜZELTME: Bu mod seçildiğinde, diğer modların bayrakları 'false' olarak kalır.
        // Sadece 'mainMode' anahtar olarak kullanılacaktır.
    }

    EndModal(wxID_OK);
}

void ModeSelectionDialog::UpdateControlStates() {
    int selection = m_mainModeChooser->GetSelection();
    
    m_singleModeSizer->GetStaticBox()->Enable(selection == 0);
    m_radioBoxSingleChoice->Enable(selection == 0);

    m_multiModeSizer->GetStaticBox()->Enable(selection == 1);
    m_checkBC->Enable(selection == 1);
    m_checkRT->Enable(selection == 1);
    m_checkBM->Enable(selection == 1);
    
    m_scenarioModeSizer->GetStaticBox()->Enable(selection == 2);

    Layout();
}