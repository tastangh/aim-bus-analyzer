// fileName: ui/frameComponent.cpp
#include "frameComponent.hpp"
#include "BCPanel.hpp"
#include "createFrameWindow.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

FrameComponent::FrameComponent(wxWindow *parent, BCPanel* mainPanel, const FrameConfig &config)
    : wxPanel(parent, wxID_ANY), m_config(config), m_mainPanel(mainPanel) {
    
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
    
    SetSizerAndFit(mainSizer);

    sendButton->Bind(wxEVT_BUTTON, &FrameComponent::onSend, this);
    m_activateToggle->Bind(wxEVT_TOGGLEBUTTON, &FrameComponent::onActivateToggle, this);
    editButton->Bind(wxEVT_BUTTON, &FrameComponent::onEdit, this);
    removeButton->Bind(wxEVT_BUTTON, &FrameComponent::onRemove, this);

    updateValues(config);
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
            m_dataLabels[i]->Show(false);
        }
    }
    if (m_mainPanel) m_mainPanel->updateListLayout();
}

void FrameComponent::sendFrame() {
    std::cout << "[UI] FrameComponent::sendFrame called for: " << m_config.label << " (BACKEND NOT IMPLEMENTED)" << std::endl;
    if (m_mainPanel) m_mainPanel->setStatusText("Sent frame '" + m_config.label + "' (simulated).");
}

bool FrameComponent::isActive() const { return m_activateToggle->GetValue(); }
void FrameComponent::onSend(wxCommandEvent &) { sendFrame(); }
void FrameComponent::onActivateToggle(wxCommandEvent &) { m_activateToggle->SetLabel(m_activateToggle->GetValue() ? "Active" : "Activate"); }
void FrameComponent::onEdit(wxCommandEvent &) { auto *frame = new CreateFrameWindow(m_mainPanel, this); frame->Show(true); }
void FrameComponent::onRemove(wxCommandEvent &) { if (m_mainPanel) { m_mainPanel->removeFrame(this); } }