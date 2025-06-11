#include "frameComponent.hpp"
#include <string>
#include <utility>
#include <sstream>
#include <iomanip>
#include "bc.hpp"
#include "createFrameWindow.hpp"
#include "logger.hpp"
#include "mainWindow.hpp"
#include "Api1553.h"

// ... Constructor aynı kalabilir ...
FrameComponent::FrameComponent(wxWindow *parent, const std::string &label, char bus, int rt, int rt2, int sa, int sa2,
                               int wc, BcMode mode, std::array<std::string, RT_SA_MAX_COUNT> data)
    : wxPanel(parent, wxID_ANY), m_mainWindow(dynamic_cast<BusControllerFrame *>(parent->GetParent())) {
  
    m_allText = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
    auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *actionSizer = new wxBoxSizer(wxVERTICAL);

    m_activateToggle = new wxToggleButton(this, wxID_ANY, "Activate", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
    auto *editFrameButton = new wxButton(this, wxID_ANY, "Edit", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
    auto *sendButton = new wxButton(this, wxID_ANY, "Send Once", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
    sendButton->SetBackgroundColour(wxColour(0, 150, 0));
    sendButton->SetForegroundColour(*wxWHITE);
    auto* removeButton = new wxButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
    
    actionSizer->Add(m_activateToggle, 0, wxALL, 2);
    actionSizer->Add(editFrameButton, 0, wxALL, 2);
    actionSizer->Add(sendButton, 0, wxALL, 2);
    actionSizer->Add(removeButton, 0, wxALL, 2);

    mainSizer->Add(m_allText, 1, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    mainSizer->Add(actionSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    SetSizerAndFit(mainSizer);

    sendButton->Bind(wxEVT_BUTTON, &FrameComponent::onSend, this);
    m_activateToggle->Bind(wxEVT_TOGGLEBUTTON, &FrameComponent::onActivateToggle, this);
    editFrameButton->Bind(wxEVT_BUTTON, &FrameComponent::onEdit, this);
    removeButton->Bind(wxEVT_BUTTON, &FrameComponent::onRemove, this);

    updateValues(label, bus, rt, rt2, sa, sa2, wc, mode, std::move(data));
}


void FrameComponent::updateValues(const std::string &label, char bus, int rt, int rt2, int sa, int sa2, int wc,
                                  BcMode mode, std::array<std::string, RT_SA_MAX_COUNT> data) {
    m_label = label; m_bus = bus; m_rt = rt; m_rt2 = rt2; m_sa = sa; m_sa2 = sa2; m_wc = wc; m_mode = mode; m_data = std::move(data);
    
    std::string text = m_label + "\n";
    text += "Bus: " + std::string(1, m_bus) + " | ";
    if (m_mode == BcMode::BC_TO_RT) text += "BC -> RT " + std::to_string(m_rt);
    if (m_mode == BcMode::RT_TO_BC) text += "RT " + std::to_string(m_rt) + " -> BC";
    if (m_mode == BcMode::RT_TO_RT) text += "RT " + std::to_string(m_rt) + " -> RT " + std::to_string(m_rt2);
    text += " | SA: " + std::to_string(m_sa) + " | WC: " + std::to_string(m_wc);
    
    m_allText->SetLabel(text);
    if(m_mainWindow) m_mainWindow->updateList();
}

void FrameComponent::sendFrame() {
    if (!m_mainWindow) return;

    m_mainWindow->setStatusText("Sending: " + m_label);
    
    std::array<AiUInt16, RT_SA_MAX_COUNT> received_u16_data;
    AiReturn status = BC::getInstance().sendAcyclicFrame(m_mode, m_bus, m_rt, m_sa, m_rt2, m_sa2, m_wc, m_data, received_u16_data);

    if (status != API_OK) {
        std::string errMsg = "Error sending frame (" + m_label + ")";
        Logger::error(errMsg);
        m_mainWindow->setStatusText(errMsg);
    } else {
        std::string logMsg = "Sent frame (" + m_label + ")";
        Logger::debug(logMsg);
        m_mainWindow->setStatusText(logMsg);
        if (m_mode == BcMode::RT_TO_BC || m_mode == BcMode::RT_TO_RT) {
            std::array<std::string, RT_SA_MAX_COUNT> new_str_data;
            int count = (m_wc == 0) ? 32 : m_wc;
            for(int i = 0; i < count; ++i) {
                std::stringstream ss;
                ss << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << received_u16_data.at(i);
                new_str_data.at(i) = m_data.at(i); // Önce eskiyi kopyala
                new_str_data.at(i) = ss.str(); // Sonra yeniyi üzerine yaz
            }
            updateData(new_str_data);
        }
    }
}

void FrameComponent::updateData(const std::array<std::string, RT_SA_MAX_COUNT>& data) {
    // UI thread'inde olduğundan emin ol
    wxTheApp->CallAfter([this, data] {
        updateValues(m_label, m_bus, m_rt, m_rt2, m_sa, m_sa2, m_wc, m_mode, data);
    });
}

bool FrameComponent::isActive() const { return m_activateToggle->GetValue(); }

void FrameComponent::onSend(wxCommandEvent &) { sendFrame(); }

void FrameComponent::onRemove(wxCommandEvent &) { 
    if (m_mainWindow) {
        int index = m_mainWindow->getFrameIndex(this);
        if (index != -1) {
            m_mainWindow->GetSizer()->Detach(index);
            m_mainWindow->updateList();
        }
    }
    this->Destroy();
}

void FrameComponent::onEdit(wxCommandEvent &) { auto *frame = new FrameCreationFrame(m_mainWindow, this); frame->Show(true); }
void FrameComponent::onActivateToggle(wxCommandEvent &) { m_activateToggle->SetLabel(m_activateToggle->GetValue() ? "Active" : "Activate"); }
void FrameComponent::onUp(wxCommandEvent &) { if (m_mainWindow) m_mainWindow->moveUp(this); }
void FrameComponent::onDown(wxCommandEvent &) { if (m_mainWindow) m_mainWindow->moveDown(this); }