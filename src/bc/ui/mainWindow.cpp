// fileName: mainWindow.cpp
#include "mainWindow.hpp"
#include "createFrameWindow.hpp"
#include "frameComponent.hpp"
#include "bc.hpp"
#include <iostream>

BusControllerFrame::BusControllerFrame()
    : wxFrame(nullptr, wxID_ANY, "AIM MIL-STD-1553 Bus Controller", wxDefaultPosition, wxSize(800, 600)) {
  
  auto *menuFile = new wxMenu;
  menuFile->Append(wxID_ADD, "Add Frame\tCtrl-A");
  menuFile->Append(wxID_CLEAR, "Clear All Frames\tCtrl-W");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  auto *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  SetMenuBar(menuBar);

  auto *topPanel = new wxPanel(this);
  auto *topSizer = new wxBoxSizer(wxHORIZONTAL);
  
  m_deviceIdTextInput = new wxTextCtrl(topPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1));
  m_repeatToggle = new wxToggleButton(topPanel, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, -1));
  m_sendActiveFramesToggle = new wxToggleButton(topPanel, wxID_ANY, "Send Active Frames", wxDefaultPosition, wxSize(170, -1));
  auto *addButton = new wxButton(topPanel, wxID_ADD, "Add Frame");

  topSizer->Add(new wxStaticText(topPanel, wxID_ANY, "AIM Device ID:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
  topSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
  topSizer->AddStretchSpacer();
  topSizer->Add(m_repeatToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  topSizer->Add(m_sendActiveFramesToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  topSizer->Add(addButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  topPanel->SetSizer(topSizer);

  m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY);
  m_scrolledSizer = new wxBoxSizer(wxVERTICAL);
  m_scrolledWindow->SetSizer(m_scrolledSizer);
  m_scrolledWindow->SetScrollRate(0, 10);
  m_scrolledWindow->FitInside();

  auto *mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);
  mainSizer->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
  SetSizer(mainSizer);

  Bind(wxEVT_MENU, &BusControllerFrame::onAddFrameClicked, this, wxID_ADD);
  Bind(wxEVT_MENU, &BusControllerFrame::onClearFramesClicked, this, wxID_CLEAR);
  Bind(wxEVT_MENU, &BusControllerFrame::onExit, this, wxID_EXIT);
  Bind(wxEVT_CLOSE_WINDOW, &BusControllerFrame::onCloseFrame, this);
  addButton->Bind(wxEVT_BUTTON, &BusControllerFrame::onAddFrameClicked, this);
  m_repeatToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerFrame::onRepeatToggle, this);
  m_sendActiveFramesToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerFrame::onSendActiveFramesToggle, this);

  CreateStatusBar();
  setStatusText("Ready. Please add frames.");
  Centre();
}

BusControllerFrame::~BusControllerFrame() {
  stopSendingThread();
}

// DÜZELTME: Eksik olan fonksiyon gövdesi eklendi.
void BusControllerFrame::onAddFrameClicked(wxCommandEvent &) {
    std::cout << "[UI] 'Add Frame' tıklandı. Yeni çerçeve oluşturma penceresi açılıyor." << std::endl;
    auto *frame = new FrameCreationFrame(this);
    frame->Show(true);
}

void BusControllerFrame::sendActiveFramesLoop() {
    auto promise_ptr = std::make_shared<std::promise<std::vector<FrameComponent*>>>();
    std::future<std::vector<FrameComponent*>> future = promise_ptr->get_future();
    wxTheApp->CallAfter([this, promise_ptr]() {
        std::vector<FrameComponent*> activeFrames;
        for (auto* frame : m_frameComponents) {
            if (frame && frame->isActive()) { activeFrames.push_back(frame); }
        }
        promise_ptr->set_value(activeFrames);
    });
    std::vector<FrameComponent*> activeFrames = future.get();
    auto& bc = BusController::getInstance();
    if (!bc.isInitialized()) {
        AiReturn ret = bc.initialize(getDeviceId());
        if (ret != API_OK) {
            wxTheApp->CallAfter([this, ret]{ wxMessageBox("Failed to initialize AIM device: " + wxString(BusController::getAIMError(ret)), "Error", wxOK | wxICON_ERROR); stopSendingThread(); });
            return;
        }
    }
    if (activeFrames.empty()) {
        wxTheApp->CallAfter([this] { setStatusText("No active frames to send. Stopping."); stopSendingThread(); });
        return;
    }
    do {
        for (FrameComponent* frame : activeFrames) {
            if (!m_isSending) break;
            frame->sendFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        if (m_isSending && m_repeatToggle->GetValue()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(BC_FRAME_TIME_MS));
        }
    } while (m_isSending && m_repeatToggle->GetValue());
    if (m_isSending) {
        stopSendingThread();
    }
}
void BusControllerFrame::onClearFramesClicked(wxCommandEvent &) { 
    if (m_isSending) {
        wxMessageBox("Please stop sending frames before clearing the list.", "Warning", wxOK | wxICON_WARNING);
        return; 
    }
    for (auto* comp : m_frameComponents) {
        // wxWidgets, bir pencere silindiğinde sizer'dan otomatik olarak kaldırır.
        comp->Destroy(); 
    } 
    m_frameComponents.clear(); 
    updateListLayout(); 
    setStatusText("All frames cleared."); 
}

void BusControllerFrame::onRepeatToggle(wxCommandEvent &) {
    m_repeatToggle->SetLabel(m_repeatToggle->GetValue() ? "Repeat On" : "Repeat Off");
}

void BusControllerFrame::onSendActiveFramesToggle(wxCommandEvent &) {
  if (m_sendActiveFramesToggle->GetValue()) {
    m_sendActiveFramesToggle->SetLabel("Sending...");
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
    if(this) {
        m_sendActiveFramesToggle->SetValue(false);
        m_sendActiveFramesToggle->SetLabel("Send Active Frames");
        setStatusText("Sending stopped.");
    }
  });
}

void BusControllerFrame::addFrameToList(const FrameConfig &config) {
  std::cout << "[UI] Yeni çerçeve listeye ekleniyor: " << config.label << std::endl;
  auto *component = new FrameComponent(m_scrolledWindow, config);
  m_frameComponents.push_back(component);
  m_scrolledSizer->Add(component, 0, wxEXPAND | wxALL, 5);
  updateListLayout();
}

void BusControllerFrame::removeFrame(FrameComponent* frame) {
    if (!frame) return;
    std::cout << "[UI] Çerçeve listeden ve sizer'dan kaldırılıyor." << std::endl;
    m_scrolledSizer->Detach(frame);
    m_frameComponents.erase(std::remove(m_frameComponents.begin(), m_frameComponents.end(), frame), m_frameComponents.end());
    // Destroy çağrısı pencereyi ve kaynaklarını güvenli bir şekilde siler.
    // CallAfter, olası thread sorunlarını önler.
    wxTheApp->CallAfter([frame](){ frame->Destroy(); });
    updateListLayout();
}

void BusControllerFrame::updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig) {
    oldFrame->updateValues(newConfig);
}

void BusControllerFrame::updateListLayout() {
  m_scrolledSizer->Layout();
  m_scrolledWindow->FitInside();
}

void BusControllerFrame::setStatusText(const wxString &status) { 
    if(this) SetStatusText(status); 
}

int BusControllerFrame::getDeviceId() { 
    long val = 0;
    if (m_deviceIdTextInput) m_deviceIdTextInput->GetValue().ToLong(&val);
    return (int)val;
}

void BusControllerFrame::onExit(wxCommandEvent &) { Close(true); }

void BusControllerFrame::onCloseFrame(wxCloseEvent &) {
  stopSendingThread();
  BusController::getInstance().shutdown();
  Destroy();
}