#include "FrameComponent.hpp"
#include "BusControllerPanel.hpp"
#include "CreateFrameWindow.hpp"
#include "bc.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/app.h>       // HATA ÇÖZÜMÜ: wxTheApp için bu satır eklendi.
#include <wx/msgdlg.h>    // HATA ÇÖZÜMÜ: wxMessageBox için bu satır eklendi.

#include <sstream>
#include <iomanip>
#include <iostream>

FrameComponent::FrameComponent(wxWindow *parent, const FrameConfig &config)
    : wxPanel(parent, wxID_ANY), m_config(config), m_aimTransferId(0), m_aimHeaderId(0), m_aimBufferId(0) {
    
    m_parentPanel = dynamic_cast<BusControllerPanel *>(parent->GetParent());
    
    auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *leftSizer = new wxBoxSizer(wxVERTICAL);

    m_summaryText = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
    leftSizer->Add(m_summaryText, 0, wxEXPAND | wxBOTTOM, 5);

    auto* dataGridSizer = new wxGridSizer(8, wxSize(5, 5));
    for(int i = 0; i < BC_MAX_DATA_WORDS; ++i) {
        m_dataLabels[i] = new wxStaticText(this, wxID_ANY, "----");
        m_dataLabels[i]->SetFont(wxFontInfo(8).Family(wxFONTFAMILY_TELETYPE));
        dataGridSizer->Add(m_dataLabels[i], 0, wxALIGN_CENTER);
    }
    leftSizer->Add(dataGridSizer, 1, wxEXPAND | wxLEFT, 10);
    
    auto *actionSizer = new wxBoxSizer(wxVERTICAL);
    m_activateToggle = new wxToggleButton(this, wxID_ANY, "Activate", wxDefaultPosition, wxSize(120, -1));
    auto *editButton = new wxButton(this, wxID_ANY, "Edit", wxDefaultPosition, wxSize(120, -1));
    auto *sendButton = new wxButton(this, wxID_ANY, "Send Once", wxDefaultPosition, wxSize(120, -1));
    auto *removeButton = new wxButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxSize(120, -1));

    actionSizer->Add(m_activateToggle, 0, wxALL, 2);
    actionSizer->Add(editButton, 0, wxALL, 2);
    actionSizer->Add(sendButton, 0, wxALL, 2);
    actionSizer->Add(removeButton, 0, wxALL, 2);

    mainSizer->Add(leftSizer, 1, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    mainSizer->Add(actionSizer, 0, wxALIGN_TOP | wxALL, 5);
    
    SetSizer(mainSizer);

    sendButton->Bind(wxEVT_BUTTON, &FrameComponent::onSend, this);
    m_activateToggle->Bind(wxEVT_TOGGLEBUTTON, &FrameComponent::onActivateToggle, this);
    editButton->Bind(wxEVT_BUTTON, &FrameComponent::onEdit, this);
    removeButton->Bind(wxEVT_BUTTON, &FrameComponent::onRemove, this);

    updateValues(config);
}

void FrameComponent::setAimIds(AiUInt16 xferId, AiUInt16 hdrId, AiUInt16 bufId) {
    m_aimTransferId = xferId;
    m_aimHeaderId = hdrId;
    m_aimBufferId = bufId;
}

void FrameComponent::updateValues(const FrameConfig &config) {
    m_config = config;
    std::stringstream ss;
    ss << m_config.label << "\n";
    ss << "Bus: " << m_config.bus << " | ";
    bool isMc = (m_config.mode == BcMode::MODE_CODE_NO_DATA || m_config.mode == BcMode::MODE_CODE_WITH_DATA);
    switch (m_config.mode) {
    case BcMode::BC_TO_RT: ss << "BC -> RT " << m_config.rt << " | SA " << m_config.sa << " | WC " << m_config.wc; break;
    case BcMode::RT_TO_BC: ss << "RT " << m_config.rt << " -> BC" << " | SA " << m_config.sa << " | WC " << m_config.wc; break;
    case BcMode::RT_TO_RT: ss << "RT " << m_config.rt << "(SA" << m_config.sa << ") -> RT " << m_config.rt2 << "(SA" << m_config.sa2 << ") | WC " << m_config.wc; break;
    default: ss << "BC -> RT " << m_config.rt << " | MC " << m_config.sa << (m_config.mode == BcMode::MODE_CODE_WITH_DATA ? " (w/ data)" : ""); break;
    }
    m_summaryText->SetLabel(ss.str());

    int wordsToShow = 0;
    if (!isMc) {
        wordsToShow = (m_config.wc == 0) ? 32 : m_config.wc;
    } else if (m_config.mode == BcMode::MODE_CODE_WITH_DATA) {
        wordsToShow = 1;
    }

    for(int i = 0; i < BC_MAX_DATA_WORDS; ++i) {
        if (i < wordsToShow) {
            m_dataLabels[i]->SetLabel(m_config.data[i]);
            m_dataLabels[i]->Show(true);
        } else {
            m_dataLabels[i]->SetLabel("");
            m_dataLabels[i]->Show(false);
        }
    }
    if (m_parentPanel) m_parentPanel->updateListLayout();
}

void FrameComponent::updateDataUI(const std::array<AiUInt16, BC_MAX_DATA_WORDS>& newData) {
    wxTheApp->CallAfter([this, newData]{
        if (!this) return;
        FrameConfig newConfig = m_config;
        std::array<std::string, BC_MAX_DATA_WORDS> new_str_data;
        int count = (m_config.wc == 0) ? 32 : m_config.wc;
        for(int i = 0; i < count; ++i) {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << newData.at(i);
            new_str_data.at(i) = ss.str();
        }
        newConfig.data = new_str_data;
        updateValues(newConfig);
    });
}

void FrameComponent::sendFrame() {
    if (!m_parentPanel) { std::cerr << "[UI] HATA: Ana panel bulunamadı!" << std::endl; return; }
    
    auto& bc = BusController::getInstance();
    if (!bc.isInitialized()) {
        AiReturn ret = bc.initialize(m_parentPanel->getDeviceId());
        if (ret != API_OK) {
            wxMessageBox("Failed to initialize AIM device: " + wxString(bc.getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
            return;
        }
    }

    wxTheApp->CallAfter([this]{ if(this && m_parentPanel) m_parentPanel->setStatusText("Sending: " + m_config.label); });
    
    std::array<AiUInt16, BC_MAX_DATA_WORDS> received_data;
    AiReturn status = bc.sendAcyclicFrame(this, received_data);

    if (status != API_OK) {
        std::string errMsg = "Error sending frame '" + m_config.label + "': " + std::string(bc.getAIMError(status));
        wxTheApp->CallAfter([this, errMsg]{ if(this && m_parentPanel) m_parentPanel->setStatusText(errMsg); });
    } else {
        std::string logMsg = "Sent frame '" + m_config.label + "' successfully.";
        wxTheApp->CallAfter([this, logMsg]{ if(this && m_parentPanel) m_parentPanel->setStatusText(logMsg); });
        if (m_config.mode == BcMode::RT_TO_BC || m_config.mode == BcMode::RT_TO_RT) {
            updateDataUI(received_data);
        }
    }
}

bool FrameComponent::isActive() const { 
    return m_activateToggle->GetValue(); 
}

void FrameComponent::onSend(wxCommandEvent &) { 
    sendFrame(); 
}

void FrameComponent::onActivateToggle(wxCommandEvent &) { 
    m_activateToggle->SetLabel(m_activateToggle->GetValue() ? "Active" : "Activate"); 
}

void FrameComponent::onEdit(wxCommandEvent &) { 
    if (!m_parentPanel) return;
    auto *frame = new FrameCreationFrame(m_parentPanel, this); 
    frame->Show(true); 
}

void FrameComponent::onRemove(wxCommandEvent &) { 
    if (m_parentPanel) { 
        m_parentPanel->removeFrame(this); 
    } 
}