// fileName: ui/createFrameWindow.cpp
#include "createFrameWindow.hpp"
#include "BCPanel.hpp"
#include "frameComponent.hpp"

#include <sstream>
#include <iomanip>
#include <random>

CreateFrameWindow::CreateFrameWindow(BCPanel *parent)
    : wxFrame(parent, wxID_ANY, "Create New 1553 Frame"), m_parentPanel(parent) {
    createAndLayoutControls();
    m_saveButton->SetLabel("Add Frame");
    updateControlStates();
}

CreateFrameWindow::CreateFrameWindow(BCPanel *parent, FrameComponent *frameToEdit)
    : wxFrame(parent, wxID_ANY, "Edit 1553 Frame"), m_parentPanel(parent), m_editingFrame(frameToEdit) {
    createAndLayoutControls();
    populateFieldsFromConfig(m_editingFrame->getFrameConfig());
    m_saveButton->SetLabel("Save Changes");
    updateControlStates();
}

void CreateFrameWindow::createAndLayoutControls() {
    auto *mainSizer = new wxBoxSizer(wxVERTICAL);
    auto *topSizer = new wxBoxSizer(wxHORIZONTAL);
    m_cmdWord2Sizer = new wxBoxSizer(wxHORIZONTAL);
    auto *dataGridSizer = new wxGridSizer(4, 8, 5, 5);
    auto *labelSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    m_saveButton = new wxButton(this, wxID_SAVE, "Save");
    auto *closeButton = new wxButton(this, wxID_CANCEL, "Cancel");
    auto *randomizeButton = new wxButton(this, wxID_ANY, "Randomize Data");

    wxArrayString rtSaWcOptions;
    for(int i=0; i<32; ++i) rtSaWcOptions.Add(std::to_string(i));
    wxString busOptions[] = {"A", "B"};
    wxString modeOptions[] = {"BC->RT", "RT->BC", "RT->RT", "Mode Code (No Data)", "Mode Code (With Data)"};

    m_busCombo = new wxComboBox(this, wxID_ANY, "A", wxDefaultPosition, wxDefaultSize, 2, busOptions, wxCB_READONLY);
    m_rtCombo = new wxComboBox(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_rt2Combo = new wxComboBox(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_saCombo = new wxComboBox(this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_sa2Combo = new wxComboBox(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_wcCombo = new wxComboBox(this, wxID_ANY, "32", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_modeCombo = new wxComboBox(this, wxID_ANY, modeOptions[0], wxDefaultPosition, wxDefaultSize, 5, modeOptions, wxCB_READONLY);
    m_labelTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
    m_labelTextCtrl->SetHint("Set frame label");

    for (int i = 0; i < BC_MAX_DATA_WORDS; ++i) {
        auto *data = new wxTextCtrl(this, wxID_ANY, "0000", wxDefaultPosition, wxSize(70, -1));
        m_dataTextCtrls.push_back(data);
        dataGridSizer->Add(data, 0, wxEXPAND);
    }
    
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Bus:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_busCombo, 0, wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Mode:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_modeCombo, 0, wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "RT:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_rtCombo, 0, wxRIGHT, 10);
    
    m_saLabel = new wxStaticText(this, wxID_ANY, "SA:");
    topSizer->Add(m_saLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); 
    topSizer->Add(m_saCombo, 0, wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "WC:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); 
    topSizer->Add(m_wcCombo, 0, wxRIGHT, 10);
    
    m_cmdWord2Sizer->Add(new wxStaticText(this, wxID_ANY, "RT2:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); 
    m_cmdWord2Sizer->Add(m_rt2Combo, 0, wxRIGHT, 10);
    m_cmdWord2Sizer->Add(new wxStaticText(this, wxID_ANY, "SA2:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); 
    m_cmdWord2Sizer->Add(m_sa2Combo, 0, wxRIGHT, 10);

    labelSizer->Add(new wxStaticText(this, wxID_ANY, "Label: "), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);
    labelSizer->Add(m_labelTextCtrl, 1, wxEXPAND);
    
    bottomSizer->Add(randomizeButton, 0, wxALL, 5);
    bottomSizer->AddStretchSpacer();
    bottomSizer->Add(closeButton, 0, wxALL, 5);
    bottomSizer->Add(m_saveButton, 0, wxALL, 5);

    mainSizer->Add(topSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(m_cmdWord2Sizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Data (Hex):"), 0, wxLEFT|wxTOP, 10);
    mainSizer->Add(dataGridSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(labelSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    mainSizer->Add(bottomSizer, 0, wxEXPAND | wxALL, 5);

    SetSizerAndFit(mainSizer);
    Centre();
  
    Bind(wxEVT_BUTTON, &CreateFrameWindow::onSave, this, wxID_SAVE);
    Bind(wxEVT_BUTTON, &CreateFrameWindow::onClose, this, wxID_CANCEL);
    Bind(wxEVT_BUTTON, &CreateFrameWindow::onRandomize, this, randomizeButton->GetId());
    m_wcCombo->Bind(wxEVT_COMBOBOX, &CreateFrameWindow::onWcChanged, this);
    m_modeCombo->Bind(wxEVT_COMBOBOX, &CreateFrameWindow::onModeChanged, this);
}

void CreateFrameWindow::populateFieldsFromConfig(const FrameConfig& config) {
    m_labelTextCtrl->SetValue(config.label);
    m_busCombo->SetValue(wxString::FromAscii(config.bus));
    m_modeCombo->SetSelection(static_cast<int>(config.mode));
    m_rtCombo->SetValue(std::to_string(config.rt));
    m_saCombo->SetValue(std::to_string(config.sa));
    m_rt2Combo->SetValue(std::to_string(config.rt2));
    m_sa2Combo->SetValue(std::to_string(config.sa2));
    m_wcCombo->SetValue(std::to_string(config.wc));
    for (size_t i = 0; i < m_dataTextCtrls.size(); ++i) {
        if (i < config.data.size()) { m_dataTextCtrls.at(i)->SetValue(config.data.at(i)); }
    }
}

FrameConfig CreateFrameWindow::buildConfigFromFields() {
    std::array<std::string, BC_MAX_DATA_WORDS> data;
    for (size_t i = 0; i < data.size(); ++i) { data.at(i) = m_dataTextCtrls.at(i)->GetValue().ToStdString(); }
    std::string label = m_labelTextCtrl->GetValue().ToStdString();
    if (label.empty()) { label = "Untitled Frame"; }
    long rt, sa, rt2, sa2, wc;
    m_rtCombo->GetValue().ToLong(&rt);
    m_saCombo->GetValue().ToLong(&sa);
    m_rt2Combo->GetValue().ToLong(&rt2);
    m_sa2Combo->GetValue().ToLong(&sa2);
    m_wcCombo->GetValue().ToLong(&wc);
    return { label, m_busCombo->GetValue().ToStdString()[0], (int)rt, (int)sa, (int)rt2, (int)sa2, (int)wc, static_cast<BcMode>(m_modeCombo->GetSelection()), data };
}

void CreateFrameWindow::onSave(wxCommandEvent &) {
    FrameConfig config = buildConfigFromFields();
    if (m_editingFrame) { m_parentPanel->updateFrame(m_editingFrame, config); } 
    else { m_parentPanel->addFrameToList(config); }
    Close(true);
}

void CreateFrameWindow::onModeChanged(wxCommandEvent &) { updateControlStates(); }
void CreateFrameWindow::onWcChanged(wxCommandEvent &) { updateControlStates(); }
void CreateFrameWindow::onClose(wxCommandEvent &) { Close(true); }

void CreateFrameWindow::updateControlStates() {
    BcMode currentMode = static_cast<BcMode>(m_modeCombo->GetSelection());
    bool isRtToRt = (currentMode == BcMode::RT_TO_RT);
    bool isMc = (currentMode == BcMode::MODE_CODE_NO_DATA || currentMode == BcMode::MODE_CODE_WITH_DATA);
    
    m_cmdWord2Sizer->ShowItems(isRtToRt);
    m_saLabel->SetLabel(isMc ? "MC:" : "SA:");
    m_wcCombo->Enable(!isMc);
    
    bool dataEnabled = (currentMode != BcMode::MODE_CODE_NO_DATA);
    long wc_long = 0;
    m_wcCombo->GetValue().ToLong(&wc_long);
    int wc = isMc ? 1 : (int)wc_long;
    if (wc == 0 && !isMc) wc = 32;

    for (size_t i = 0; i < m_dataTextCtrls.size(); ++i) {
        m_dataTextCtrls[i]->Enable(dataEnabled && (i < (size_t)wc));
    }
    
    GetSizer()->Layout();
    Fit();
}

void CreateFrameWindow::onRandomize(wxCommandEvent &) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 65535);
  for (auto &dataTextCtrl : m_dataTextCtrls) {
    std::stringstream ss;
    ss << std::uppercase << std::setw(4) << std::setfill('0') << std::hex << distr(eng);
    dataTextCtrl->SetValue(ss.str());
  }
}