// fileName: ui/ScenarioPanel.cpp
#include "ScenarioPanel.hpp"
#include "mainWindow.hpp"
#include <wx/font.h>

ScenarioPanel::ScenarioPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY) {
    m_mainFrame = (MainWindow*) parent->GetParent();

    auto* sizer = new wxBoxSizer(wxVERTICAL);

    // DÜZELTME: Device ID, Start butonu vb. için üst kontrol barı oluşturuldu.
    auto* topSizer = new wxBoxSizer(wxHORIZONTAL);
    
    topSizer->Add(new wxStaticText(this, wxID_ANY, "AIM Device ID:"), 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    m_deviceIdTextInput = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1));
    topSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    
    topSizer->AddStretchSpacer(); // Butonları sağa yaslamak için boşluk

    m_startButton = new wxButton(this, wxID_ANY, "Start Scenario");
    topSizer->Add(m_startButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    sizer->Add(topSizer, 0, wxEXPAND | wxTOP | wxRIGHT, 5);

    wxStaticText* infoText = new wxStaticText(this, wxID_ANY, 
        "This mode runs a predefined concurrent BC, RT, and BM simulation.\n"
        "Click 'Start Scenario' to begin. All traffic will be logged below and visible on an external Bus Monitor."
    );
    infoText->Wrap(500);
    sizer->Add(infoText, 0, wxALL | wxEXPAND, 10);

    m_logText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_DONTWRAP);
    wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_logText->SetFont(font);
    
    sizer->Add(m_logText, 1, wxEXPAND | wxALL, 10);
    
    this->SetSizer(sizer);

    m_startButton->Bind(wxEVT_BUTTON, &ScenarioPanel::OnStartScenario, this);
}

void ScenarioPanel::OnStartScenario(wxCommandEvent& event) {
    // Backend kodu eklendiğinde buradan Device ID okunacak.
    long deviceId = 0;
    m_deviceIdTextInput->GetValue().ToLong(&deviceId);

    m_logText->AppendText(wxString::Format("Scenario Started on Device %ld (BACKEND NOT IMPLEMENTED)\n", deviceId));
    m_startButton->Disable();
    m_deviceIdTextInput->Disable();
    if(m_mainFrame) m_mainFrame->SetStatusText("Scenario Running...");
}