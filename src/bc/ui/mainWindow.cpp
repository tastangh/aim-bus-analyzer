#include "mainWindow.hpp"
#include "createFrameWindow.hpp"
#include "frameComponent.hpp"
#include "bc.hpp"
#include <iostream>
#include <algorithm> 

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

// ... addFrameToList, updateFrame, removeFrame, onAddFrameClicked, onClearFramesClicked, onRepeatToggle aynı kalacak ...
void BusControllerFrame::addFrameToList(FrameConfig config) {
    std::cout << "[UI] Yeni çerçeve ekleniyor: " << config.label << std::endl;
    auto& bc = BusController::getInstance();
    if (!bc.isInitialized()) {
        std::cout << "[UI] BC başlatılmamış, initialize çağrılıyor..." << std::endl;
        AiReturn ret = bc.initialize(getDeviceId());
        if (ret != API_OK) {
            wxMessageBox("Failed to initialize AIM device: " + wxString(BusController::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
            return;
        }
    }
    
    auto *component = new FrameComponent(m_scrolledWindow, config);
    AiReturn ret = bc.defineFrameResources(component);
    if (ret != API_OK) {
        wxMessageBox("Failed to define frame resources on AIM device: " + wxString(BusController::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
        component->Destroy(); 
        return;
    }
    
    m_frameComponents.push_back(component);
    m_scrolledSizer->Add(component, 0, wxEXPAND | wxALL, 5);
    updateListLayout();
}

void BusControllerFrame::updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig) {
    std::cout << "[UI] Çerçeve güncelleniyor: " << newConfig.label << std::endl;
    removeFrame(oldFrame);
    addFrameToList(newConfig);
}

void BusControllerFrame::removeFrame(FrameComponent* frame) {
    if (!frame) return;
    std::cout << "[UI] Çerçeve listeden kaldırılıyor: " << frame->getFrameConfig().label << std::endl;
    
    m_scrolledSizer->Detach(frame);
    m_frameComponents.erase(std::remove(m_frameComponents.begin(), m_frameComponents.end(), frame), m_frameComponents.end());
    updateListLayout();
    wxTheApp->CallAfter([frame](){ frame->Destroy(); });
}

void BusControllerFrame::onAddFrameClicked(wxCommandEvent &) {
    std::cout << "[UI] 'Add Frame' tıklandı. Yeni çerçeve oluşturma penceresi açılıyor." << std::endl;
    auto *frame = new FrameCreationFrame(this);
    frame->Show(true);
}

void BusControllerFrame::onClearFramesClicked(wxCommandEvent &) {
    if (m_isSending) {
        wxMessageBox("Please stop sending frames before clearing the list.", "Warning", wxOK | wxICON_WARNING);
        return;
    }
    auto components_to_delete = m_frameComponents;
    for (auto* comp : components_to_delete) {
        removeFrame(comp);
    }
    setStatusText("All frames cleared.");
}

void BusControllerFrame::onRepeatToggle(wxCommandEvent &) {
    m_repeatToggle->SetLabel(m_repeatToggle->GetValue() ? "Repeat On" : "Repeat Off");
}


// --- DEĞİŞİKLİKLER BU BÖLÜMDE ---

void BusControllerFrame::onSendActiveFramesToggle(wxCommandEvent &event) {
  if (m_sendActiveFramesToggle->GetValue()) {
    // DÜZELTME: Gönderim zaten aktifse tekrar başlatma
    if (m_isSending) return; 
    
    // DÜZELTME: Butonu hemen devre dışı bırakıp etiketini değiştirerek çift tıklamayı önle
    m_sendActiveFramesToggle->SetLabel("Sending...");
    m_sendActiveFramesToggle->Disable();
    startSendingThread();
  } else {
    // Butona tekrar basıldığında (durdurma isteği)
    m_sendActiveFramesToggle->Disable(); // Durdurma işlemi bitene kadar butonu kilitle
    stopSendingThread();
  }
}

void BusControllerFrame::startSendingThread() {
  if (m_isSending) return;
  m_isSending = true;
  // Thread bittiğinde dahi join edilebilir kalması için detach yerine join kullanacağız.
  m_sendThread = std::thread(&BusControllerFrame::sendActiveFramesLoop, this);
}

void BusControllerFrame::stopSendingThread() {
  // Sinyali gönder
  m_isSending = false;

  // Thread'in bitmesini bekle
  if (m_sendThread.joinable()) {
    m_sendThread.join();
  }
  
  // DÜZELTME: Arayüz güncellemelerini her zaman wxTheApp->CallAfter ile yap
  // Bu, worker thread'den çağrıldığında bile güvenli olmasını sağlar.
  wxTheApp->CallAfter([this] {
    if(this) { // Pencerenin hala var olduğundan emin ol
        m_sendActiveFramesToggle->SetValue(false); // Butonun durumunu 'kapalı' yap
        m_sendActiveFramesToggle->SetLabel("Send Active Frames");
        m_sendActiveFramesToggle->Enable(); // Butonu tekrar aktif et
        setStatusText("Sending stopped.");
    }
  });
}

void BusControllerFrame::sendActiveFramesLoop() {
    // Aktif çerçeveleri UI thread'inden güvenli bir şekilde al
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
            // DÜZELTME: Hata mesajı sonrası durdurma işlemini UI thread'ine bırak
            wxTheApp->CallAfter([this, ret]{ 
                wxMessageBox("Failed to initialize AIM device: " + wxString(BusController::getAIMError(ret)), "Error", wxOK | wxICON_ERROR); 
                stopSendingThread(); 
            });
            return;
        }
    }

    if (activeFrames.empty()) {
        wxTheApp->CallAfter([this] { 
            setStatusText("No active frames to send. Stopping."); 
            stopSendingThread(); 
        });
        return;
    }
    
    // Ana gönderim döngüsü
    do {
        for (FrameComponent* frame : activeFrames) {
            if (!m_isSending) break; // Her çerçeve öncesi durdurma sinyalini kontrol et
            frame->sendFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        if (!m_isSending) break; // Döngü tekrar etmeden önce son bir kez daha kontrol et
        
        // Tekrar modu aktifse, bir sonraki döngü için bekle
        if (m_repeatToggle->GetValue()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(BC_FRAME_TIME_MS));
        }
    } while (m_isSending && m_repeatToggle->GetValue());

    // DÜZELTME (ANA ÇÖZÜM): Thread işini bitirince, kendi kendini durdurmaya çalışmak yerine
    // durdurma ve temizleme işini ana GUI thread'ine havale et.
    wxTheApp->CallAfter([this] {
        if (m_isSending) { // Eğer kullanıcı butona basarak durdurmadıysa, biz durduralım
           stopSendingThread();
        }
    });
}

// ... updateListLayout, setStatusText, getDeviceId, onExit, onCloseFrame aynı kalacak ...
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