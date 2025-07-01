// fileName: ui/mainWindow.cpp
#include "mainWindow.hpp"
#include "createFrameWindow.hpp"
#include "frameComponent.hpp"
#include "rt.hpp" // DÜZELTME: Bu satır eklendi.
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
    
    m_deviceIdText = new wxTextCtrl(topPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1));
    m_repeatToggle = new wxToggleButton(topPanel, wxID_ANY, "Repeat Off", wxDefaultPosition, wxSize(100, -1));
    m_startStopToggle = new wxToggleButton(topPanel, wxID_ANY, "Start Simulation", wxDefaultPosition, wxSize(140, -1));
    auto *addButton = new wxButton(topPanel, wxID_ADD, "Add Frame");

    m_rtAddressText = new wxTextCtrl(this, wxID_ANY, "1");
    m_rtAddressText->Hide();

    topSizer->Add(new wxStaticText(topPanel, wxID_ANY, "AIM Device ID:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    topSizer->Add(m_deviceIdText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    topSizer->AddStretchSpacer();
    topSizer->Add(m_repeatToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topSizer->Add(m_startStopToggle, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topSizer->Add(addButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topPanel->SetSizer(topSizer);

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

void