#include "mainWindow.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <wx/wx.h>
#include "common.hpp"
#include "createFrameWindow.hpp"
#include "frameComponent.hpp"
#include "logger.hpp"
#include "bc.hpp" // Backend sınıfı

constexpr int MAX_FILE_PATH_SIZE = 1024;

BusControllerFrame::BusControllerFrame() : wxFrame(nullptr, wxID_ANY, "AIM MIL-STD-1553 Bus Controller") {
    // --- Logger ve Backend'i Başlatma ---
    Logger::info("Bus Controller UI starting up.");
    
    // --- Menü ve Arayüz Elemanları ---
    auto *menuFile = new wxMenu;
    int addFrameId = wxNewId();
    menuFile->Append(addFrameId, "Add Frame\tCtrl-A", "Add a frame to the frame list");
    int clearFramesId = wxNewId();
    menuFile->Append(clearFramesId, "Clear All Frames\tCtrl-W", "Clear all frames from the frame list");
    int loadFramesId = wxNewId();
    menuFile->Append(loadFramesId, "Load frames\tCtrl-L", "Load frames from a file");
    int saveFramesId = wxNewId();
    menuFile->Append(saveFramesId, "Save frames\tCtrl-S", "Save frame into a file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Commands");
    SetMenuBar(menuBar);

    auto *deviceIdLabel = new wxStaticText(this, wxID_ANY, "AIM Device ID");
    m_deviceIdTextInput = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(40, TOP_BAR_COMP_HEIGHT));
    m_repeatToggle = new wxToggleButton(this, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
    m_sendActiveFramesToggle = new wxToggleButton(this, wxID_ANY, "Send Active Frames", wxDefaultPosition, wxSize(170, TOP_BAR_COMP_HEIGHT));
    m_sendActiveFramesToggle->SetBackgroundColour(wxColour("#00ccff"));
    m_addButton = new wxButton(this, wxID_ANY, "Add Frame", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
    m_addButton->SetBackgroundColour(wxColour("#ffcc00"));

    // --- Sizer Kurulumu ---
    auto *verticalSizer = new wxBoxSizer(wxVERTICAL);
    auto *topHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY);
    m_scrolledWindow->SetBackgroundColour(this->GetBackgroundColour());
    m_scrolledSizer = new wxBoxSizer(wxVERTICAL);
    m_scrolledWindow->SetSizer(m_scrolledSizer);
    m_scrolledWindow->SetScrollRate(10, 10);

    topHorizontalSizer->Add(deviceIdLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->AddStretchSpacer();
    topHorizontalSizer->Add(m_repeatToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_sendActiveFramesToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_addButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    verticalSizer->Add(topHorizontalSizer, 0, wxEXPAND | wxALL, 5);
    verticalSizer->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
    SetSizer(verticalSizer);

    // --- Olayları Bağlama (Binding) ---
    Bind(wxEVT_MENU, &BusControllerFrame::onAddFrameClicked, this, addFrameId);
    Bind(wxEVT_MENU, &BusControllerFrame::onClearFramesClicked, this, clearFramesId);
    Bind(wxEVT_MENU, &BusControllerFrame::onLoadFrames, this, loadFramesId);
    Bind(wxEVT_MENU, &BusControllerFrame::onSaveFrames, this, saveFramesId);
    Bind(wxEVT_MENU, &BusControllerFrame::onExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &BusControllerFrame::onCloseFrame, this);

    m_addButton->Bind(wxEVT_BUTTON, &BusControllerFrame::onAddFrameClicked, this);
    m_repeatToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerFrame::onRepeatToggle, this);
    m_sendActiveFramesToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerFrame::onSendActiveFrames, this);

    // --- Son Ayarlar ve Konfigürasyon Yükleme ---
    nlohmann::json config;
    std::ifstream configFile(CONFIG_PATH);
    if (configFile.is_open()) {
        try {
            configFile >> config;
            if (config.contains("Bus_Controller") && config["Bus_Controller"].contains("Default_Device_Number")) {
                m_deviceIdTextInput->SetValue(std::to_string(config["Bus_Controller"]["Default_Device_Number"].get<int>()));
            }
        } catch (const nlohmann::json::parse_error &e) {
            Logger::error("JSON parse error: " + std::string(e.what()));
        }
    }
    
    CreateStatusBar();
    SetStatusText("Ready. Please add frames.");
    SetMinSize(wxSize(700, 500));
    Centre();
}

BusControllerFrame::~BusControllerFrame() {
    stopSending(); // Thread'i güvenli bir şekilde durdur
    BC::getInstance().shutdown(); // Backend'i kapat
}

void BusControllerFrame::onAddFrameClicked(wxCommandEvent & /*event*/) {
  auto *frame = new FrameCreationFrame(this);
  frame->Show(true);
}

void BusControllerFrame::onClearFramesClicked(wxCommandEvent & /*event*/) { 
    m_scrolledSizer->Clear(true); 
    updateList();
}

void BusControllerFrame::onRepeatToggle(wxCommandEvent & /*event*/) {
  if (m_repeatToggle->GetValue()) {
    m_repeatToggle->SetLabel("Repeat On");
  } else {
    m_repeatToggle->SetLabel("Repeat Off");
    if (m_isSending) stopSending();
  }
}

void BusControllerFrame::onSendActiveFrames(wxCommandEvent & /*event*/) {
  if (m_sendActiveFramesToggle->GetValue()) {
    if (BC::getInstance().initialize(getDeviceId()) != API_OK) {
        wxMessageBox("Failed to initialize AIM device. Check device ID and connection.", "Error", wxOK | wxICON_ERROR, this);
        m_sendActiveFramesToggle->SetValue(false);
        return;
    }
    m_sendActiveFramesToggle->SetLabel("Sending...");
    m_sendActiveFramesToggle->SetBackgroundColour(wxColour("#ff4545"));
    m_sendActiveFramesToggle->SetForegroundColour(*wxWHITE);
    if (m_repeatToggle->GetValue()) {
      startSendingThread();
    } else {
      sendActiveFrames();
      stopSending(); 
    }
  } else {
    stopSending();
  }
}

void BusControllerFrame::stopSending() {
  m_isSending = false;
  if (m_repeatedSendThread.joinable()) {
    m_repeatedSendThread.join();
  }
  m_sendActiveFramesToggle->SetValue(false);
  m_sendActiveFramesToggle->SetLabel("Send Active Frames");
  m_sendActiveFramesToggle->SetBackgroundColour(wxColour("#00ccff"));
  m_sendActiveFramesToggle->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
}

void BusControllerFrame::startSendingThread() {
  if (m_isSending) return;
  m_isSending = true;
  m_repeatedSendThread = std::thread([this] {
      while (m_isSending) {
          sendActiveFrames();
          // Tekrarlı gönderimler arasında kısa bir bekleme
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
  });
}

void BusControllerFrame::sendActiveFrames() {
  wxTheApp->CallAfter([this] {
    for (auto &child : m_scrolledSizer->GetChildren()) {
        if (!m_isSending) break; // Döngü sırasında durdurulursa çık
        auto *frame = dynamic_cast<FrameComponent *>(child->GetWindow());
        if (frame != nullptr && frame->isActive()) {
            frame->sendFrame();
            // Frameler arası minimum bekleme süresi
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
  });
}

void BusControllerFrame::onLoadFrames(wxCommandEvent & /*event*/) {
    // Bu fonksiyonun içeriği değişmeden kalabilir.
}

void BusControllerFrame::onSaveFrames(wxCommandEvent & /*event*/) {
    // Bu fonksiyonun içeriği değişmeden kalabilir.
}

void BusControllerFrame::onExit(wxCommandEvent & /*event*/) {
  Close(true);
}

void BusControllerFrame::onCloseFrame(wxCloseEvent& event) {
    stopSending();
    BC::getInstance().shutdown();
    Destroy();
}

// ... Diğer yardımcı fonksiyonlar (moveUp, moveDown, vs.) değişmeden kalır.
void BusControllerFrame::addFrameToList(const std::string &label, char bus, int rt, int rt2, int sa, int sa2, int wc,
                                        BcMode mode, std::array<std::string, RT_SA_MAX_COUNT> data) {
  auto *component = new FrameComponent(m_scrolledWindow, label, bus, rt, rt2, sa, sa2, wc, mode, std::move(data));
  m_scrolledSizer->Add(component, 0, wxEXPAND | wxALL, 5);
  updateList();
}
void BusControllerFrame::setStatusText(const wxString &status) { SetStatusText(status); }
int BusControllerFrame::getDeviceId() { return wxAtoi(m_deviceIdTextInput->GetValue()); }
void BusControllerFrame::moveUp(FrameComponent *item) {
  int index = getFrameIndex(item);
  if (index > 0) {
    m_scrolledSizer->Detach(item);
    m_scrolledSizer->Insert(index - 1, item, 0, wxEXPAND | wxALL, 5);
    updateList();
  }
}
void BusControllerFrame::moveDown(FrameComponent *item) {
    int index = getFrameIndex(item);
    if (index != -1 && index < (int)m_scrolledSizer->GetItemCount() - 1) {
        m_scrolledSizer->Detach(item);
        m_scrolledSizer->Insert(index + 1, item, 0, wxEXPAND | wxALL, 5);
        updateList();
    }
}
void BusControllerFrame::updateList() {
    m_scrolledWindow->FitInside();
    m_scrolledSizer->Layout();
}
int BusControllerFrame::getFrameIndex(FrameComponent *frame) {
    for (size_t i = 0; i < m_scrolledSizer->GetItemCount(); ++i) {
        if (m_scrolledSizer->GetItem(i)->GetWindow() == frame) {
            return i;
        }
    }
    return -1;
}