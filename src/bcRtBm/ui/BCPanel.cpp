// fileName: ui/BCPanel.cpp
#include "BCPanel.hpp"
#include "frameComponent.hpp"
#include "createFrameWindow.hpp"
#include "mainWindow.hpp" // setStatusText için
#include <algorithm>
#include <iostream>

BCPanel::BCPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    m_mainFrame = (MainWindow*)parent->GetParent()->GetParent(); // Notebook -> Sizer -> MainWindow

    auto *topSizer = new wxBoxSizer(wxHORIZONTAL);
  
    m_deviceIdTextInput = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1));
    m_repeatToggle = new wxToggleButton(this, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, -1));
    m_sendActiveFramesToggle = new wxToggleButton(this, wxID_ANY, "Send Active Frames", wxDefaultPosition, wxSize(170, -1));
    auto *addButton = new wxButton(this, wxID_ADD, "Add Frame");

    topSizer->Add(new wxStaticText(this, wxID_ANY, "AIM Device ID:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    topSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    topSizer->AddStretchSpacer();
    topSizer->Add(m_repeatToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topSizer->Add(m_sendActiveFramesToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topSizer->Add(addButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY);
    m_scrolledSizer = new wxBoxSizer(wxVERTICAL);
    m_scrolledWindow->SetSizer(m_scrolledSizer);
    m_scrolledWindow->SetScrollRate(0, 10);
    m_scrolledWindow->FitInside();

    auto *panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(topSizer, 0, wxEXPAND | wxALL, 5);
    panelSizer->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
    this->SetSizer(panelSizer);

    addButton->Bind(wxEVT_BUTTON, &BCPanel::onAddFrameClicked, this);
    m_repeatToggle->Bind(wxEVT_TOGGLEBUTTON, &BCPanel::onRepeatToggle, this);
    m_sendActiveFramesToggle->Bind(wxEVT_TOGGLEBUTTON, &BCPanel::onSendActiveFramesToggle, this);
}

BCPanel::~BCPanel() {
    // stopSendingThread(); // Backend eklendiğinde açılacak
}

void BCPanel::addFrameToList(const FrameConfig& config) {
    std::cout << "[UI] BC Panel: Adding frame - " << config.label << std::endl;
    // Backend eklendiğinde buraya bc.initialize() ve defineFrameResources() çağrıları gelecek.
    
    auto *component = new FrameComponent(m_scrolledWindow, this, config);
    m_frameComponents.push_back(component);
    m_scrolledSizer->Add(component, 0, wxEXPAND | wxALL, 5);
    updateListLayout();
    setStatusText("Frame '" + config.label + "' added.");
}

void BCPanel::updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig) {
    std::cout << "[UI] BC Panel: Updating frame - " << newConfig.label << std::endl;
    // Backend eklendiğinde buraya kaynakları silip yeniden oluşturma mantığı gelecek.
    oldFrame->updateValues(newConfig);
    setStatusText("Frame '" + newConfig.label + "' updated.");
}

void BCPanel::removeFrame(FrameComponent* frame) {
    if (!frame) return;
    m_scrolledSizer->Detach(frame);
    m_frameComponents.erase(std::remove(m_frameComponents.begin(), m_frameComponents.end(), frame), m_frameComponents.end());
    updateListLayout();
    wxTheApp->CallAfter([frame](){ frame->Destroy(); });
    setStatusText("Frame removed.");
}

void BCPanel::updateListLayout() {
    this->Layout();
    m_scrolledWindow->FitInside();
}

int BCPanel::getDeviceId() {
    long val = 0;
    if (m_deviceIdTextInput) m_deviceIdTextInput->GetValue().ToLong(&val);
    return (int)val;
}

void BCPanel::setStatusText(const wxString& text) {
    if (m_mainFrame) {
        m_mainFrame->SetStatusText(text);
    }
}

void BCPanel::onAddFrameClicked(wxCommandEvent &) {
    auto *frame = new CreateFrameWindow(this);
    frame->Show(true);
}

void BCPanel::onClearFramesClicked(wxCommandEvent &) {
    // Backend eklendiğinde m_isSending kontrolü eklenecek.
    auto components_to_delete = m_frameComponents;
    for (auto* comp : components_to_delete) {
        removeFrame(comp);
    }
    setStatusText("All frames cleared.");
}

void BCPanel::onRepeatToggle(wxCommandEvent &) {
    m_repeatToggle->SetLabel(m_repeatToggle->GetValue() ? "Repeat On" : "Repeat Off");
}

void BCPanel::onSendActiveFramesToggle(wxCommandEvent &) {
    // Backend eklendiğinde burası doldurulacak.
    if(m_sendActiveFramesToggle->GetValue()) {
        setStatusText("Sending Started (BACKEND NOT IMPLEMENTED)");
    } else {
        setStatusText("Sending Stopped (BACKEND NOT IMPLEMENTED)");
    }
}

// Backend fonksiyonları şimdilik boş
void BCPanel::startSendingThread() {}
void BCPanel::stopSendingThread() {}
void BCPanel::sendActiveFramesLoop() {}