// fileName: ui/ModeSelectionDialog.cpp
#include "ModeSelectionDialog.hpp"
#include <wx/statline.h> // DÜZELTME: wxStaticLine için başlık dosyası eklendi.

ModeSelectionDialog::ModeSelectionDialog(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // --- Mod Tipi Seçimi ---
    wxBoxSizer* modeTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    m_radioSingleMode = new wxRadioButton(this, wxID_ANY, "Single-Mode Operation", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    m_radioMultiMode = new wxRadioButton(this, wxID_ANY, "Multi-Mode Operation");
    modeTypeSizer->Add(m_radioSingleMode, 0, wxALL, 10);
    modeTypeSizer->Add(m_radioMultiMode, 0, wxALL, 10);
    mainSizer->Add(modeTypeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
    mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    // --- Tekli Mod Seçenekleri ---
    wxArrayString singleModeOptions;
    singleModeOptions.Add("Bus Controller (BC)");
    singleModeOptions.Add("Remote Terminal (RT)");
    singleModeOptions.Add("Bus Monitor (BM)");
    m_radioBoxSingleChoice = new wxRadioBox(this, wxID_ANY, "Select a single mode", wxDefaultPosition, wxDefaultSize, singleModeOptions, 1, wxRA_SPECIFY_COLS);
    
    m_singleModeSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Single-Mode");
    m_singleModeSizer->Add(m_radioBoxSingleChoice, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(m_singleModeSizer, 0, wxEXPAND | wxALL, 10);

    // --- Çoklu Mod Seçenekleri ---
    m_multiModeSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Multi-Mode");
    m_checkBC = new wxCheckBox(this, wxID_ANY, "Bus Controller (BC)");
    m_checkRT = new wxCheckBox(this, wxID_ANY, "Remote Terminal (RT)");
    m_checkBM = new wxCheckBox(this, wxID_ANY, "Bus Monitor (BM)");
    m_multiModeSizer->Add(m_checkBC, 0, wxALL, 5);
    m_multiModeSizer->Add(m_checkRT, 0, wxALL, 5);
    m_multiModeSizer->Add(m_checkBM, 0, wxALL, 5);
    mainSizer->Add(m_multiModeSizer, 0, wxEXPAND | wxALL, 10);

    // --- OK / Cancel Butonları ---
    mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);

    // Olayları bağla
    m_radioSingleMode->Bind(wxEVT_RADIOBUTTON, &ModeSelectionDialog::OnModeTypeChange, this);
    m_radioMultiMode->Bind(wxEVT_RADIOBUTTON, &ModeSelectionDialog::OnModeTypeChange, this);
    Bind(wxEVT_BUTTON, &ModeSelectionDialog::OnOK, this, wxID_OK);
    
    // Başlangıç durumu
    m_radioSingleMode->SetValue(true);
    UpdateControlStates();

    SetSizerAndFit(mainSizer);
    Centre();
}

void ModeSelectionDialog::OnModeTypeChange(wxCommandEvent& event) {
    UpdateControlStates();
}

void ModeSelectionDialog::OnOK(wxCommandEvent& event) {
    // Seçimleri AppConfig yapısına kaydet
    m_config.isMultiMode = m_radioMultiMode->GetValue();
    if (m_config.isMultiMode) {
        m_config.bc_selected = m_checkBC->IsChecked();
        m_config.rt_selected = m_checkRT->IsChecked();
        m_config.bm_selected = m_checkBM->IsChecked();
        m_config.singleModeSelection = -1; // Geçersiz
    } else {
        m_config.singleModeSelection = m_radioBoxSingleChoice->GetSelection();
        m_config.bc_selected = (m_config.singleModeSelection == 0);
        m_config.rt_selected = (m_config.singleModeSelection == 1);
        m_config.bm_selected = (m_config.singleModeSelection == 2);
    }
    EndModal(wxID_OK); // Diyalogu kapat ve OK döndür
}

void ModeSelectionDialog::UpdateControlStates() {
    bool isSingleMode = m_radioSingleMode->GetValue();
    m_singleModeSizer->GetStaticBox()->Enable(isSingleMode);
    m_radioBoxSingleChoice->Enable(isSingleMode);

    m_multiModeSizer->GetStaticBox()->Enable(!isSingleMode);
    m_checkBC->Enable(!isSingleMode);
    m_checkRT->Enable(!isSingleMode);
    m_checkBM->Enable(!isSingleMode);

    // Layout'u yeniden hesapla
    Layout();
}