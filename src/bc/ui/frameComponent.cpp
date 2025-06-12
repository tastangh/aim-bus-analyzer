#include "frameComponent.hpp"
#include "../bc.hpp"
#include "createFrameWindow.hpp"
#include "logger.hpp"
#include "mainWindow.hpp"
#include <iomanip>
#include <sstream>

FrameComponent::FrameComponent(wxWindow *parent, const FrameConfig &config)
    : wxPanel(parent, wxID_ANY), m_config(config),
      m_mainWindow(dynamic_cast<BusControllerFrame *>(parent->GetParent())) {

  m_allText = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
  auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
  auto *actionSizer = new wxBoxSizer(wxVERTICAL);

  m_activateToggle = new wxToggleButton(this, wxID_ANY, "Activate", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
  auto *editFrameButton = new wxButton(this, wxID_ANY, "Edit", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
  auto *sendButton = new wxButton(this, wxID_ANY, "Send Once", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));
  auto *removeButton = new wxButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));

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

  updateValues(config);
}

void FrameComponent::updateValues(const FrameConfig &config) {
  m_config = config;
  std::stringstream ss;
  ss << m_config.label << "\n";
  ss << "Bus: " << m_config.bus << " | ";

  switch (m_config.mode) {
  case BcMode::BC_TO_RT:
    ss << "BC -> RT " << m_config.rt << " | SA: " << m_config.sa << " | WC: " << m_config.wc;
    break;
  case BcMode::RT_TO_BC:
    ss << "RT " << m_config.rt << " -> BC" << " | SA: " << m_config.sa << " | WC: " << m_config.wc;
    break;
  case BcMode::RT_TO_RT:
    ss << "RT " << m_config.rt << "(SA" << m_config.sa << ") -> RT " << m_config.rt2 << "(SA" << m_config.sa2 << ") | WC: " << m_config.wc;
    break;
  case BcMode::MODE_CODE_NO_DATA:
  case BcMode::MODE_CODE_WITH_DATA:
    ss << "BC -> RT " << m_config.rt << " | MC: " << m_config.sa << (m_config.mode == BcMode::MODE_CODE_WITH_DATA ? " (with data)" : " (no data)");
    break;
  }
  m_allText->SetLabel(ss.str());
  if (m_mainWindow) m_mainWindow->updateListLayout();
}

void FrameComponent::updateData(const std::array<AiUInt16, BC_MAX_DATA_WORDS>& newData) {
    wxTheApp->CallAfter([this, newData] {
        std::array<std::string, BC_MAX_DATA_WORDS> new_str_data = m_config.data;
        int count = (m_config.wc == 0) ? 32 : m_config.wc;
        for(int i = 0; i < count; ++i) {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << newData.at(i);
            new_str_data.at(i) = ss.str();
        }
        FrameConfig newConfig = {m_config.label, m_config.bus, m_config.rt, m_config.sa, m_config.rt2, m_config.sa2, m_config.wc, m_config.mode, new_str_data};
        updateValues(newConfig);
    });
}

void FrameComponent::sendFrame() {
  if (!m_mainWindow) return;
  
  if (!BC::getInstance().isInitialized()) {
      AiReturn ret = BC::getInstance().initialize(m_mainWindow->getDeviceId());
      if (ret != API_OK) {
        wxMessageBox("Failed to initialize AIM device: " + wxString(BC::getInstance().getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
        return;
      }
  }

  m_mainWindow->setStatusText("Sending: " + m_config.label);
  std::array<AiUInt16, BC_MAX_DATA_WORDS> received_data;
  
  AiReturn status = BC::getInstance().sendFrame(m_config, received_data);

  if (status != API_OK) {
    std::string errMsg = "Error sending frame (" + m_config.label + "): " + BC::getInstance().getAIMError(status);
    Logger::error(errMsg);
    m_mainWindow->setStatusText(errMsg);
  } else {
    std::string logMsg = "Sent frame (" + m_config.label + ") successfully.";
    Logger::debug(logMsg);
    m_mainWindow->setStatusText(logMsg);
    if (m_config.mode == BcMode::RT_TO_BC || m_config.mode == BcMode::RT_TO_RT) {
        updateData(received_data);
    }
  }
}

bool FrameComponent::isActive() const { return m_activateToggle->GetValue(); }
void FrameComponent::onSend(wxCommandEvent &) { sendFrame(); }
void FrameComponent::onActivateToggle(wxCommandEvent &) { m_activateToggle->SetLabel(m_activateToggle->GetValue() ? "Active" : "Activate"); }
void FrameComponent::onEdit(wxCommandEvent &) { auto *frame = new FrameCreationFrame(m_mainWindow, this); frame->Show(true); }
void FrameComponent::onRemove(wxCommandEvent &) { 
    if (m_mainWindow) {
        m_mainWindow->removeFrame(this);
    }
    this->Destroy();
}