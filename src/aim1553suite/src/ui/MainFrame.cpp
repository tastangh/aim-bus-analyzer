#include "MainFrame.hpp"
#include "BusControllerPanel.hpp"
#include "BusMonitorPanel.hpp"
#include "DeviceSelectionPanel.hpp"
#include "bc.hpp"
#include "bm.hpp"
#include <wx/sizer.h>

enum { ID_Exit = 1 };

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "AIM 1553 Suite - Hardware Setup", wxDefaultPosition, wxSize(800, 600)) {
    
    // HATA ÇÖZÜMÜ: Durum çubuğunu burada, en başta oluşturuyoruz.
    CreateStatusBar();
    SetStatusText("Please scan for hardware.");

    SetMenuBar(nullptr);
    
    auto* sizer = new wxBoxSizer(wxVERTICAL);
    
    m_devicePanel = new DeviceSelectionPanel(this);
    sizer->Add(m_devicePanel, 1, wxEXPAND);

    m_notebook = new wxNotebook(this, wxID_ANY);
    m_bcPanel = new BusControllerPanel(m_notebook);
    m_bmPanel = new BusMonitorPanel(m_notebook);
    m_notebook->AddPage(m_bcPanel, "Bus Controller");
    m_notebook->AddPage(m_bmPanel, "Bus Monitor");
    sizer->Add(m_notebook, 1, wxEXPAND);
    m_notebook->Hide();

    SetSizer(sizer);
    
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::onClose, this);
    
    Centre();
}

void MainFrame::InitializePanels(unsigned int deviceId, unsigned int bcStreamId, unsigned int bmStreamId) {
    m_bcPanel->InitializeHardware(deviceId, bcStreamId);
    m_bmPanel->InitializeHardware(deviceId, bmStreamId);

    m_devicePanel->Hide();
    m_notebook->Show();
    GetSizer()->Layout();
    SetTitle(wxString::Format("AIM 1553 Suite - Device %u", deviceId));

    auto *menuFile = new wxMenu;
    menuFile->Append(ID_Exit, "E&xit\tAlt-X");
    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);
    Bind(wxEVT_MENU, &MainFrame::onExit, this, ID_Exit);
}

void MainFrame::onExit(wxCommandEvent& event) {
    Close(true); 
}

void MainFrame::onClose(wxCloseEvent& event) {
    Destroy();
}