// fileName: ui/mainWindow.cpp
#include "mainWindow.hpp"
#include "createFrameWindow.hpp"
#include "frameComponent.hpp"
#include "rt.hpp"
#include <algorithm> 

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "AIM MIL-STD-1553 Simulator", wxDefaultPosition, wxSize(800, 600)) {
  
    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_ADD, "Add Frame\tCtrl-A");
    menuFile->Append(wxID_CLEAR, "Clear All\tCtrl-W");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    auto *topPanel = new wxPanel(this);
    auto *topSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // BC arayüzündeki gibi kontroller oluşturuluyor
    m_deviceIdText = new wxTextCtrl(topPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1));
    m_repeatToggle = new wxToggleButton(topPanel, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, -1));
    m_startStopToggle = new wxToggleButton(topPanel, wxID_ANY, "Start Simulation", wxDefaultPosition, wxSize(140, -1));
    auto *addButton = new wxButton(topPanel, wxID_ADD, "Add Frame");

    // RT adresi için bir text kutusu oluşturuluyor ancak arayüzde gösterilmiyor,
    // sadece değeri saklamak için kullanılıyor. Değiştirmek isterseniz görünür yapabilirsiniz.
    m_rtAddressText = new wxTextCtrl(this, wxID_ANY, "1");
    m_rtAddressText->Hide();

    topSizer->Add(new wxStaticText(topPanel, wxID_ANY, "AIM Device ID:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    topSizer->Add(m_deviceIdText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    topSizer->AddStretchSpacer();
    topSizer->Add(m_repeatToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topSizer->Add(m_startStopToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topSizer->Add(addButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topPanel->SetSizer(topSizer);

    // "Repeat" düğmesi RT mantığında anlamsız olduğu için devre dışı bırakılıyor.
    m_repeatToggle->Disable();

    m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY);
    m_scrolledSizer = new wxBoxSizer(wxVERTICAL);
    m_scrolledWindow->SetSizer(m_scrolledSizer);
    m_scrolledWindow->SetScrollRate(0, 10);
    m_scrolledWindow->FitInside();

    auto *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
    SetSizer(mainSizer);

    Bind(wxEVT_MENU, &MainWindow::onAdd, this, wxID_ADD);
    Bind(wxEVT_MENU, &MainWindow::onClear, this, wxID_CLEAR);
    Bind(wxEVT_MENU, &MainWindow::onExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onCloseFrame, this);
    m_startStopToggle->Bind(wxEVT_TOGGLEBUTTON, &MainWindow::onStartStopToggle, this);
    addButton->Bind(wxEVT_BUTTON, &MainWindow::onAdd, this);

    CreateStatusBar();
    SetStatusText("Ready. Configure Frames (RT->BC Transmit SA) and Start Simulation.");
    Centre();
}

MainWindow::~MainWindow() {
    RemoteTerminal::getInstance().shutdown();
}

void MainWindow::addFrame(FrameConfig config) {
    auto* component = new FrameComponent(m_scrolledWindow, config);
    if (RemoteTerminal::getInstance().isRunning()) {
        AiReturn ret = RemoteTerminal::getInstance().defineFrameAsSubaddress(component);
        if (ret != API_OK) {
            wxMessageBox("Failed to define frame on AIM device: " + wxString(RemoteTerminal::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
            component->Destroy();
            return;
        }
    }
    m_frameComponents.push_back(component);
    m_scrolledSizer->Add(component, 0, wxEXPAND | wxALL, 5);
    updateListLayout();
}

void MainWindow::updateFrame(FrameComponent* component, const FrameConfig& newConfig) {
    if (!component) return;
    component->updateValues(newConfig);
    if (RemoteTerminal::getInstance().isRunning()) {
        AiReturn ret = RemoteTerminal::getInstance().defineFrameAsSubaddress(component);
        if (ret != API_OK) {
            wxMessageBox("Failed to update frame on AIM device: " + wxString(RemoteTerminal::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainWindow::removeFrame(FrameComponent* component) {
    if (!component) return;
    m_scrolledSizer->Detach(component);
    m_frameComponents.erase(std::remove(m_frameComponents.begin(), m_frameComponents.end(), component), m_frameComponents.end());
    updateListLayout();
    wxTheApp->CallAfter([component](){ component->Destroy(); });
}

void MainWindow::onAdd(wxCommandEvent &) {
    auto *frame = new CreateFrameWindow(this);
    frame->Show(true);
}

void MainWindow::onClear(wxCommandEvent &) {
    if (RemoteTerminal::getInstance().isRunning()) {
        wxMessageBox("Please stop the simulation before clearing all frames.", "Warning", wxOK | wxICON_WARNING);
        return;
    }
    auto components_to_delete = m_frameComponents;
    for (auto* comp : components_to_delete) {
        removeFrame(comp);
    }
    SetStatusText("All frames cleared.");
}

void MainWindow::onStartStopToggle(wxCommandEvent &) {
    auto& rt = RemoteTerminal::getInstance();
    if (m_startStopToggle->GetValue()) { // Start the simulation
        m_startStopToggle->SetLabel("Running...");
        m_deviceIdText->Disable();
        
        AiReturn ret = rt.start(getDeviceId(), getRtAddress());
        if (ret != API_OK) {
            wxMessageBox("Failed to start RT simulation: " + wxString(RemoteTerminal::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
            m_startStopToggle->SetValue(false);
            m_startStopToggle->SetLabel("Start Simulation");
            m_deviceIdText->Enable(true);
            return;
        }

        for (auto* comp : m_frameComponents) {
            ret = rt.defineFrameAsSubaddress(comp);
            if (ret != API_OK) {
                wxMessageBox("Failed to configure Frame " + comp->getConfig().label + ": " + wxString(RemoteTerminal::getAIMError(ret)), "Error", wxOK | wxICON_ERROR);
                rt.shutdown();
                m_startStopToggle->SetValue(false);
                m_startStopToggle->SetLabel("Start Simulation");
                m_deviceIdText->Enable(true);
                return;
            }
        }
        SetStatusText("RT Simulation Running on Address " + std::to_string(getRtAddress()));

    } else { // Stop the simulation
        rt.shutdown();
        m_startStopToggle->SetLabel("Start Simulation");
        SetStatusText("RT Simulation Stopped.");
        m_deviceIdText->Enable(true);
    }
}

void MainWindow::updateListLayout() {
  m_scrolledSizer->Layout();
  m_scrolledWindow->FitInside();
}

int MainWindow::getDeviceId() { 
    long val = 0;
    m_deviceIdText->GetValue().ToLong(&val);
    return (int)val;
}

int MainWindow::getRtAddress() { 
    long val = 0;
    m_rtAddressText->GetValue().ToLong(&val);
    return (int)val;
}

void MainWindow::onExit(wxCommandEvent &) { Close(true); }
void MainWindow::onCloseFrame(wxCloseEvent &) {
    RemoteTerminal::getInstance().shutdown();
    Destroy();
}