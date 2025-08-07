#include "BusControllerPanel.hpp"
#include "MainFrame.hpp"
#include "CreateFrameWindow.hpp"
#include "FrameComponent.hpp"
#include "bc.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/app.h>       // HATA ÇÖZÜMÜ: wxTheApp için eklendi.
#include <wx/msgdlg.h>    // HATA ÇÖZÜMÜ: wxMessageBox için eklendi.
#include <iostream>
#include <algorithm>

BusControllerPanel::BusControllerPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {

    m_mainFrame = dynamic_cast<MainFrame*>(wxGetTopLevelParent(this));
  
    auto* topPanel = new wxPanel(this, wxID_ANY);
    auto* topSizer = new wxBoxSizer(wxHORIZONTAL);
  
    m_deviceIdTextInput = new wxTextCtrl(topPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1));
    m_repeatToggle = new wxToggleButton(topPanel, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, -1));
    m_sendActiveFramesToggle = new wxToggleButton(topPanel, wxID_ANY, "Send Active Frames", wxDefaultPosition, wxSize(170, -1));
    auto* addButton = new wxButton(topPanel, wxID_ANY, "Add Frame");

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

    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
    
    this->SetSizer(mainSizer);

    addButton->Bind(wxEVT_BUTTON, &BusControllerPanel::onAddFrameClicked, this);
    m_repeatToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerPanel::onRepeatToggle, this);
    m_sendActiveFramesToggle->Bind(wxEVT_TOGGLEBUTTON, &BusControllerPanel::onSendActiveFramesToggle, this);

    setStatusText("Bus Controller ready. Please add frames.");
}

BusControllerPanel::~BusControllerPanel() {
    stopSendingThread();
}

void BusControllerPanel::setStatusText(const wxString &status) {
    if (m_mainFrame) {
        m_mainFrame->SetStatusText(status); 
    }
}

void BusControllerPanel::addFrameToList(FrameConfig config) {
    auto& bc = BusController::getInstance();
    if (!bc.isInitialized()) {
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

void BusControllerPanel::updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig) {
    removeFrame(oldFrame);
    addFrameToList(newConfig);
}

void BusControllerPanel::removeFrame(FrameComponent* frame) {
    if (!frame) return;
    m_scrolledSizer->Detach(frame);
    m_frameComponents.erase(std::remove(m_frameComponents.begin(), m_frameComponents.end(), frame), m_frameComponents.end());
    updateListLayout();
    wxTheApp->CallAfter([frame](){ frame->Destroy(); });
}

void BusControllerPanel::onAddFrameClicked(wxCommandEvent &) {
    auto *frame = new FrameCreationFrame(this);
    frame->Show(true);
}

void BusControllerPanel::onClearFramesClicked(wxCommandEvent &) {
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

void BusControllerPanel::onRepeatToggle(wxCommandEvent &) {
    bool is_on = m_repeatToggle->GetValue();
    m_repeatToggle->SetLabel(is_on ? "Repeat On" : "Repeat Off");
    m_isRepeatOn = is_on; // HATA ÇÖZÜMÜ: Atomic bool'u thread-safe olarak güncelle.
}

void BusControllerPanel::onSendActiveFramesToggle(wxCommandEvent &event) {
    if (m_sendActiveFramesToggle->GetValue()) {
        if (m_isSending) return; 
        m_sendActiveFramesToggle->SetLabel("Sending...");
        m_sendActiveFramesToggle->Disable();
        startSendingThread();
    } else {
        m_sendActiveFramesToggle->Disable(); 
        stopSendingThread();
    }
}

void BusControllerPanel::startSendingThread() {
    if (m_isSending) return;
    m_isSending = true;
    m_isRepeatOn = m_repeatToggle->GetValue(); // Başlamadan önce durumu güvenli bir şekilde al.
    m_sendThread = std::thread(&BusControllerPanel::sendActiveFramesLoop, this);
}

void BusControllerPanel::stopSendingThread() {
    m_isSending = false;
    if (m_sendThread.joinable()) {
        m_sendThread.join();
    }
    
    wxTheApp->CallAfter([this] {
        if(this) { 
            m_sendActiveFramesToggle->SetValue(false); 
            m_sendActiveFramesToggle->SetLabel("Send Active Frames");
            m_sendActiveFramesToggle->Enable(); 
            setStatusText("Sending stopped.");
        }
    });
}

void BusControllerPanel::sendActiveFramesLoop() {
    auto promise_ptr = std::make_shared<std::promise<std::vector<FrameComponent*>>>();
    std::future<std::vector<FrameComponent*>> future = promise_ptr->get_future();
    wxTheApp->CallAfter([this, promise_ptr]() {
        if (!this) {
            // Promise'i yine de set etmeliyiz ki future.get() sonsuza kadar beklemesin.
            std::vector<FrameComponent*> empty;
            promise_ptr->set_value(empty);
            return;
        };
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
            wxTheApp->CallAfter([this, ret]{ 
                if(this) {
                    wxMessageBox("Failed to initialize AIM device: " + wxString(BusController::getAIMError(ret)), "Error", wxOK | wxICON_ERROR); 
                    stopSendingThread();
                }
            });
            return;
        }
    }
    
    if (activeFrames.empty()) {
        wxTheApp->CallAfter([this] { if(this) { setStatusText("No active frames to send. Stopping."); stopSendingThread(); }});
        return;
    }
    
    // HATA ÇÖZÜMÜ: Hatalı .Wait() çağrısı kaldırıldı ve döngü koşulu atomic bool ile güncellendi.
    do {
        for (FrameComponent* frame : activeFrames) {
            if (!m_isSending) break;
            frame->sendFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        if (!m_isSending) break; 

        if (m_isRepeatOn.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(BC_FRAME_TIME_MS));
        }
    } while (m_isSending && m_isRepeatOn.load());

    wxTheApp->CallAfter([this] {
        if (m_isSending) { 
           stopSendingThread();
        }
    });
}

void BusControllerPanel::updateListLayout() {
    m_scrolledSizer->Layout();
    m_scrolledWindow->FitInside();
    this->Layout();
}
int BusControllerPanel::getDeviceId() { 
    long val = 0;
    if (m_deviceIdTextInput) {
        m_deviceIdTextInput->GetValue().ToLong(&val);
    }
    return static_cast<int>(val);
}