// fileName: ui/frameComponent.cpp
#include "frameComponent.hpp"
#include "mainWindow.hpp"
#include "createFrameWindow.hpp"
#include "rt.hpp"
#include <sstream>

FrameComponent::FrameComponent(wxWindow *parent, const FrameConfig &config)
    : wxPanel(parent, wxID_ANY), m_config(config) {
    
    m_mainFrame = dynamic_cast<MainWindow *>(parent->GetParent());
    
    auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *leftSizer = new wxBoxSizer(wxVERTICAL);

    m_summaryText = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
    leftSizer->Add(m_summaryText, 0, wxEXPAND | wxBOTTOM, 5);

    auto* dataGridSizer = new wxGridSizer(8, wxSize(5, 5));
    for(int i = 0; i < 32; ++i) {
        m_dataLabels[i] = new wxStaticText(this, wxID_ANY, "----");
        m_dataLabels[i]->SetFont(wxFontInfo(8).Family(wxFONTFAMILY_TELETYPE));
        dataGridSizer->Add(m_dataLabels[i], 0, wxALIGN_CENTER);
    }
    leftSizer->Add(dataGridSizer, 1, wxEXPAND | wxLEFT, 10);
    
    auto *actionSizer = new wxBoxSizer(wxVERTICAL);
    m_enableToggle = new wxToggleButton(this, wxID_ANY, "Disabled", wxDefaultPosition, wxSize(120, -1));
    m_enableToggle->SetValue(config.enabled);
    auto *editButton = new wxButton(this, wxID_ANY, "Edit", wxDefaultPosition, wxSize(120, -1));
    auto *sendOnceButton = new wxButton(this, wxID_ANY, "Send Once", wxDefaultPosition, wxSize(120, -1)); // YENİ DÜĞME
    auto *removeButton = new wxButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxSize(120, -1));

    actionSizer->Add(m_enableToggle, 0, wxALL, 2);
    actionSizer->Add(editButton, 0, wxALL, 2);
    actionSizer->Add(sendOnceButton, 0, wxALL, 2); // YENİ DÜĞME EKLENDİ
    actionSizer->Add(removeButton, 0, wxALL, 2);

    mainSizer->Add(leftSizer, 1, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    mainSizer->Add(actionSizer, 0, wxALIGN_TOP | wxALL, 5);
    
    SetSizerAndFit(mainSizer);

    m_enableToggle->Bind(wxEVT_TOGGLEBUTTON, &FrameComponent::onEnableToggle, this);
    editButton->Bind(wxEVT_BUTTON, &FrameComponent::onEdit, this);
    sendOnceButton->Bind(wxEVT_BUTTON, &FrameComponent::onSendOnce, this); // YENİ BAĞLAMA
    removeButton->Bind(wxEVT_BUTTON, &FrameComponent::onRemove, this);

    updateValues(config);
}

// YENİ OLAY YÖNETİCİSİ
void FrameComponent::onSendOnce(wxCommandEvent &) {
    auto& rt = RemoteTerminal::getInstance();
    if (!rt.isRunning()) {
        wxMessageBox("RT simulation is not running. Please start the simulation first.", "Warning", wxOK | wxICON_WARNING);
        return;
    }
    
    m_mainFrame->SetStatusText("Testing SA " + m_config.label + "...");
    AiReturn ret = rt.testTransmitSubaddress(this);
    if (ret != API_OK) {
        wxMessageBox("Failed to test the frame: " + wxString(RemoteTerminal::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
        m_mainFrame->SetStatusText("Test failed for SA " + m_config.label);
    } else {
        m_mainFrame->SetStatusText("Test command for SA " + m_config.label + " sent successfully.");
    }
}

void FrameComponent::setAimIds(AiUInt16 hdrId, AiUInt16 bufId) {
    m_aimHeaderId = hdrId;
    m_aimBufferId = bufId;
}

void FrameComponent::updateValues(const FrameConfig &config) {
    m_config = config;
    
    std::stringstream ss;
    ss << m_config.label << "\n";
    ss << "Bus: " << m_config.bus << " | Mode: RT->BC | RT: " << m_config.rt << " | SA: " << m_config.sa << " | WC: " << m_config.wc;
    m_summaryText->SetLabel(ss.str());

    int wordsToShow = (m_config.wc == 0) ? 32 : m_config.wc;

    for(int i = 0; i < 32; ++i) {
        if (i < wordsToShow) {
            m_dataLabels[i]->SetLabel(m_config.data[i]);
            m_dataLabels[i]->Show(true);
        } else {
            m_dataLabels[i]->Show(false);
        }
    }
    
    m_enableToggle->SetValue(m_config.enabled);
    m_enableToggle->SetLabel(m_config.enabled ? "Enabled" : "Disabled");

    if (m_mainFrame) m_mainFrame->updateListLayout();
}

void FrameComponent::onEnableToggle(wxCommandEvent &) {
    m_config.enabled = m_enableToggle->GetValue();
    updateValues(m_config);

    if (RemoteTerminal::getInstance().isRunning()) {
        AiReturn ret = RemoteTerminal::getInstance().defineFrameAsSubaddress(this);
        if (ret != API_OK) {
            wxMessageBox("Failed to update frame state: " + wxString(RemoteTerminal::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
            m_config.enabled = !m_config.enabled; 
            updateValues(m_config);
        }
    }
}

void FrameComponent::onEdit(wxCommandEvent &) { 
    auto *frame = new CreateFrameWindow(m_mainFrame, this); 
    frame->Show(true); 
}

void FrameComponent::onRemove(wxCommandEvent &) { 
    if (m_mainFrame) { 
        m_mainFrame->removeFrame(this); 
    } 
}