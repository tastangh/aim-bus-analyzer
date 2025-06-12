#include "mainWindow.hpp"
#include "../bc.hpp"
#include "createFrameWindow.hpp"
#include "frameComponent.hpp"
#include "logger.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <utility>
#include <wx/wx.h>
#include <wx/thread.h> // wxSemaphore için gerekli

// --- Constructor ve diğer fonksiyonlar önceki gibi ---

BusControllerFrame::BusControllerFrame()
    : wxFrame(nullptr, wxID_ANY, "AIM MIL-STD-1553 Bus Controller") {
  
  Logger::info("Bus Controller UI starting up.");

  auto *menuFile = new wxMenu;
  menuFile->Append(wxID_ADD, "Add Frame\tCtrl-A");
  menuFile->Append(wxID_CLEAR, "Clear All Frames\tCtrl-W");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_OPEN, "Load Frames...\tCtrl-L");
  menuFile->Append(wxID_SAVE, "Save Frames As...\tCtrl-S");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  auto *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  SetMenuBar(menuBar);

  auto *topPanel = new wxPanel(this);
  auto *topHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
  
  auto *deviceIdLabel = new wxStaticText(topPanel, wxID_ANY, "AIM Device ID:");
  m_deviceIdTextInput = new wxTextCtrl(topPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, TOP_BAR_COMP_HEIGHT));
  m_repeatToggle = new wxToggleButton(topPanel, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
  m_sendActiveFramesToggle = new wxToggleButton(topPanel, wxID_ANY, "Send Active Frames", wxDefaultPosition, wxSize(170, TOP_BAR_COMP_HEIGHT));
  auto *addButton = new wxButton(topPanel, wxID_ADD, "Add Frame");

  topHorizontalSizer->Add(deviceIdLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
  topHorizontalSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
  topHorizontalSizer->AddStretchSpacer();
  topHorizontalSizer->Add(m_repeatToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  topHorizontalSizer->Add(m_sendActiveFramesToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  topHorizontalSizer->Add(addButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  topPanel->SetSizer(topHorizontalSizer);

  m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY);
  m_scrolledSizer = new wxBoxSizer(wxVERTICAL);
  m_scrolledWindow->SetSizer(m_scrolledSizer);
  m_scrolledWindow->SetScrollRate(0, 10);

  auto *mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);
  mainSizer->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
  SetSizer(mainSizer);

  Bind(wxEVT_MENU, &BusControllerFrame::onAddFrameClicked, this, wxID_ADD);
  Bind(wxEVT_MENU, &BusControllerFrame::onClearFramesClicked, this, wxID_CLEAR);
  Bind(wxEVT_MENU, &BusControllerFrame::onLoadFrames, this, wxID_OPEN);
  Bind(wxEVT_MENU, &BusControllerFrame::onSaveFrames, this, wxID_SAVE);
  Bind(wxEVT_MENU, &BusControllerFrame::onExit, this, wxID_EXIT);
  Bind(wxEVT_CLOSE_WINDOW, &BusControllerFrame::onCloseFrame, this);
  addButton->Bind(wxEVT_BUTTON, &BusControllerFrame::onAddFrameClicked, this);
  m_repeatToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerFrame::onRepeatToggle, this);
  m_sendActiveFramesToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerFrame::onSendActiveFramesToggle, this);

  CreateStatusBar();
  setStatusText("Ready. Please add or load frames.");
  SetMinSize(wxSize(750, 500));
  Centre();
}

BusControllerFrame::~BusControllerFrame() {
  stopSendingThread();
  BC::getInstance().shutdown();
}

void BusControllerFrame::onAddFrameClicked(wxCommandEvent &) {
  auto *frame = new FrameCreationFrame(this);
  frame->Show(true);
}

void BusControllerFrame::onClearFramesClicked(wxCommandEvent &) {
  m_scrolledSizer->Clear(true);
  updateListLayout();
  setStatusText("All frames cleared.");
}

void BusControllerFrame::onRepeatToggle(wxCommandEvent &) {
    m_repeatToggle->SetLabel(m_repeatToggle->GetValue() ? "Repeat On" : "Repeat Off");
}

void BusControllerFrame::onSendActiveFramesToggle(wxCommandEvent &) {
  if (m_sendActiveFramesToggle->GetValue()) {
    if (!BC::getInstance().isInitialized()) {
      AiReturn ret = BC::getInstance().initialize(getDeviceId());
      if (ret != API_OK) {
        wxMessageBox("Failed to initialize AIM device: " + wxString(BC::getInstance().getAIMError(ret)), "Error", wxOK | wxICON_ERROR, this);
        m_sendActiveFramesToggle->SetValue(false);
        return;
      }
    }
    m_sendActiveFramesToggle->SetLabel("Sending...");
    m_sendActiveFramesToggle->SetBackgroundColour(wxColour(220, 20, 60));
    m_sendActiveFramesToggle->SetForegroundColour(*wxWHITE);
    startSendingThread();
  } else {
    stopSendingThread();
  }
}

void BusControllerFrame::startSendingThread() {
  if (m_isSending) return;
  m_isSending = true;
  m_sendThread = std::thread(&BusControllerFrame::sendActiveFramesLoop, this);
}

void BusControllerFrame::stopSendingThread() {
  if (!m_isSending) return;
  m_isSending = false;
  if (m_sendThread.joinable()) {
    m_sendThread.join();
  }
  wxTheApp->CallAfter([this] {
    m_sendActiveFramesToggle->SetValue(false);
    m_sendActiveFramesToggle->SetLabel("Send Active Frames");
    m_sendActiveFramesToggle->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    m_sendActiveFramesToggle->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    setStatusText("Sending stopped.");
  });
}

void BusControllerFrame::sendActiveFramesLoop() {
  std::vector<FrameComponent*> frameComponents;
  wxSemaphore sem(0, 1); // Başlangıç değeri 0, maksimum 1

  wxTheApp->CallAfter([&]{
    frameComponents.clear();
    for (auto *sizerItem : m_scrolledSizer->GetChildren()) {
        auto *frame = dynamic_cast<FrameComponent *>(sizerItem->GetWindow());
        if (frame) {
            frameComponents.push_back(frame);
        }
    }
    sem.Post(); // Sinyali ver: "Liste hazır"
  });

  sem.Wait(); // Sinyalin gelmesini bekle

  if (frameComponents.empty() && m_isSending) {
    wxTheApp->CallAfter([this] { setStatusText("No frames to send. Stopping."); });
    m_isSending = false;
  }

  do {
    bool anyActiveSent = false;
    for (FrameComponent* frame : frameComponents) {
      if (!m_isSending) break;
      if (frame->isActive()) {
        anyActiveSent = true;
        frame->sendFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
      }
    }
    if (!anyActiveSent && m_isSending) {
        wxTheApp->CallAfter([this] { setStatusText("No active frames to send. Stopping."); });
        break;
    }
    if (m_isSending && m_repeatToggle->GetValue()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(BC_FRAME_TIME_MS));
    }
  } while (m_isSending && m_repeatToggle->GetValue());

  if (m_isSending) {
    stopSendingThread();
  }
}

void BusControllerFrame::onLoadFrames(wxCommandEvent &) {
    wxFileDialog openFileDialog(this, _("Open Frames JSON file"), "", "", "JSON files (*.json)|*.json", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;
    std::ifstream ifs(openFileDialog.GetPath().ToStdString());
    if (!ifs.is_open()) { wxMessageBox("Could not open file for reading.", "Error", wxOK | wxICON_ERROR); return; }
    try {
        nlohmann::json j;
        ifs >> j;
        m_scrolledSizer->Clear(true);
        for (const auto& item : j["frames"]) {
            std::array<std::string, BC_MAX_DATA_WORDS> data;
            if (item.contains("data")) {
                std::vector<std::string> vec_data = item["data"].get<std::vector<std::string>>();
                std::copy_n(vec_data.begin(), std::min(vec_data.size(), data.size()), data.begin());
            }
            FrameConfig config = {
                item.value("label", "Untitled"), item.value("bus", "A")[0],
                item.value("rt", 0), item.value("sa", 0),
                item.value("rt2", 0), item.value("sa2", 0),
                item.value("wc", 0), item.value("mode", BcMode::BC_TO_RT), data
            };
            addFrameToList(config);
        }
        setStatusText("Frames loaded from " + openFileDialog.GetFilename());
    } catch (const nlohmann::json::exception& e) {
        wxMessageBox("Error parsing JSON file: " + std::string(e.what()), "JSON Error", wxOK | wxICON_ERROR);
    }
}

void BusControllerFrame::onSaveFrames(wxCommandEvent &) {
    wxFileDialog saveFileDialog(this, _("Save Frames JSON file"), "", "", "JSON files (*.json)|*.json", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;
    nlohmann::json j;
    nlohmann::json framesArray = nlohmann::json::array();
    for (const auto& child : m_scrolledSizer->GetChildren()) {
        auto* frame = dynamic_cast<FrameComponent*>(child->GetWindow());
        if (frame) {
            const FrameConfig& config = frame->getFrameConfig();
            nlohmann::json frameJson;
            frameJson["label"] = config.label; frameJson["bus"] = std::string(1, config.bus); frameJson["mode"] = config.mode;
            frameJson["rt"] = config.rt; frameJson["sa"] = config.sa; frameJson["rt2"] = config.rt2; frameJson["sa2"] = config.sa2; frameJson["wc"] = config.wc;
            std::vector<std::string> data_vec(config.data.begin(), config.data.end());
            frameJson["data"] = data_vec;
            framesArray.push_back(frameJson);
        }
    }
    j["frames"] = framesArray;
    std::ofstream ofs(saveFileDialog.GetPath().ToStdString());
    if (ofs.is_open()) {
        ofs << j.dump(4);
        setStatusText("Frames saved to " + saveFileDialog.GetFilename());
    } else {
        wxMessageBox("Could not open file for writing.", "Error", wxOK | wxICON_ERROR);
    }
}

void BusControllerFrame::addFrameToList(const FrameConfig &config) {
  auto *component = new FrameComponent(m_scrolledWindow, config);
  m_scrolledSizer->Add(component, 0, wxEXPAND | wxALL, 5);
  updateListLayout();
}

void BusControllerFrame::removeFrame(FrameComponent* frame) {
    m_scrolledSizer->Detach(frame);
    updateListLayout();
}

void BusControllerFrame::updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig) {
    oldFrame->updateValues(newConfig);
}

void BusControllerFrame::updateListLayout() {
  m_scrolledSizer->Layout();
  m_scrolledWindow->FitInside();
}

void BusControllerFrame::setStatusText(const wxString &status) { SetStatusText(status); }
int BusControllerFrame::getDeviceId() { return wxAtoi(m_deviceIdTextInput->GetValue()); }
void BusControllerFrame::onExit(wxCommandEvent &) { Close(true); }
void BusControllerFrame::onCloseFrame(wxCloseEvent &) {
  stopSendingThread();
  Destroy();
}