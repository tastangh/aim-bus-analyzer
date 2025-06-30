// fileName: ui/createFrameWindow.cpp
#include "createFrameWindow.hpp"
#include "mainWindow.hpp"
#include "frameComponent.hpp"

#include <sstream>
#include <iomanip>
#include <random>

CreateFrameWindow::CreateFrameWindow(MainWindow *parent)
    : wxFrame(parent, wxID_ANY, "Create New RT->BC Frame"), m_mainFrame(parent) {
  createAndLayoutControls();
  m_saveButton->SetLabel("Add Frame");
}

CreateFrameWindow::CreateFrameWindow(MainWindow *parent, FrameComponent *frameToEdit)
    : wxFrame(parent, wxID_ANY, "Edit RT->BC Frame"), m_mainFrame(parent), m_editingComponent(frameToEdit) {
  createAndLayoutControls();
  populateFields(m_editingComponent->getConfig());
  m_saveButton->SetLabel("Save Changes");
}

void CreateFrameWindow::createAndLayoutControls() {
    auto *mainSizer = new wxBoxSizer(wxVERTICAL);
    auto *topSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *dataGridSizer = new wxGridSizer(4, 8, 5, 5);
    auto *labelSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    m_saveButton = new wxButton(this, wxID_SAVE, "Save");
    auto *closeButton = new wxButton(this, wxID_CANCEL, "Cancel");
    auto *randomizeButton = new wxButton(this, wxID_ANY, "Randomize Data");

    wxArrayString rtSaWcOptions;
    for(int i=0; i<32; ++i) rtSaWcOptions.Add(std::to_string(i));
    wxString busOptions[] = {"A", "B"};

    m_busCombo = new wxComboBox(this, wxID_ANY, "A", wxDefaultPosition, wxDefaultSize, 2, busOptions, wxCB_READONLY);
    m_rtCombo = new wxComboBox(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_saCombo = new wxComboBox(this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_wcCombo = new wxComboBox(this, wxID_ANY, "32", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);
    m_labelTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
    m_labelTextCtrl->SetHint("Set frame label");

    for (int i = 0; i < 32; ++i) {
        auto *data = new wxTextCtrl(this, wxID_ANY, "0000", wxDefaultPosition, wxSize(70, -1));
        m_dataTextCtrls.push_back(data);
        dataGridSizer->Add(data, 0, wxEXPAND);
    }
    
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Bus:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_busCombo, 0, wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Mode: RT->BC"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "RT:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_rtCombo, 0, wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "SA:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_saCombo, 0, wxRIGHT, 10);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "WC:"), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_wcCombo, 0, wxRIGHT, 10);

    labelSizer->Add(new wxStaticText(this, wxID_ANY, "Label: "), 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);
    labelSizer->Add(m_labelTextCtrl, 1, wxEXPAND);
    
    bottomSizer->Add(randomizeButton, 0, wxALL, 5);
    bottomSizer->AddStretchSpacer();
    bottomSizer->Add(closeButton, 0, wxALL, 5);
    bottomSizer->Add(m_saveButton, 0, wxALL, 5);

    mainSizer->Add(topSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Data (Hex):"), 0, wxLEFT|wxTOP, 10);
    mainSizer->Add(dataGridSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(labelSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    mainSizer->Add(bottomSizer, 0, wxEXPAND | wxALL, 5);

    SetSizerAndFit(mainSizer);
    Centre();
  
    Bind(wxEVT_BUTTON, &CreateFrameWindow::onSave, this, wxID_SAVE);
    Bind(wxEVT_BUTTON, &CreateFrameWindow::onClose, this, wxID_CANCEL);
    Bind(wxEVT_BUTTON, &CreateFrameWindow::onRandomize, this, randomizeButton->GetId());
}

void CreateFrameWindow::populateFields(const FrameConfig& config) {
    m_labelTextCtrl->SetValue(config.label);
    m_busCombo->SetValue(wxString::FromAscii(config.bus));
    m_rtCombo->SetValue(std::to_string(config.rt));
    m_saCombo->SetValue(std::to_string(config.sa));
    m_wcCombo->SetValue(std::to_string(config.wc));
    for (size_t i = 0; i < m_dataTextCtrls.size(); ++i) {
        if (i < config.data.size()) { m_dataTextCtrls.at(i)->SetValue(config.data.at(i)); }
    }
}

FrameConfig CreateFrameWindow::buildConfigFromFields() {
    FrameConfig config;
    long temp_val;

    config.label = m_labelTextCtrl->GetValue().ToStdString();
    if (config.label.empty()) { config.label = "Untitled RT-BC Frame"; }

    config.bus = m_busCombo->GetValue().ToStdString()[0];
    
    m_rtCombo->GetValue().ToLong(&temp_val); config.rt = temp_val;
    m_saCombo->GetValue().ToLong(&temp_val); config.sa = temp_val;
    m_wcCombo->GetValue().ToLong(&temp_val); config.wc = temp_val;

    config.enabled = m_editingComponent ? m_editingComponent->getConfig().enabled : true;

    for (size_t i = 0; i < config.data.size(); ++i) {
        config.data.at(i) = m_dataTextCtrls.at(i)->GetValue().ToStdString();
    }
    return config;
}

void CreateFrameWindow::onSave(wxCommandEvent &) {
    FrameConfig config = buildConfigFromFields();
    if (m_editingComponent) { m_mainFrame->updateFrame(m_editingComponent, config); } 
    else { m_mainFrame->addFrame(config); }
    Close(true);
}

void CreateFrameWindow::onClose(wxCommandEvent &) { Close(true); }

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