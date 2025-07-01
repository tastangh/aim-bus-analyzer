// fileName: ui/BMPanel.cpp
#include "BMPanel.hpp"
#include "mainWindow.hpp"
#include "../bm.hpp"
#include "../milStd1553.hpp"

// Gerekli diğer başlıklar
#include <wx/font.h>
#include <wx/settings.h>
#include <wx/colordlg.h>

BMPanel::BMPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    m_mainFrame = (MainWindow*) parent->GetParent();

    // --- Top Control Bar ---
    auto *deviceIdText = new wxStaticText(this, wxID_ANY, "AIM Device ID:");
    m_deviceIdTextInput = new wxTextCtrl(this, ID_DEVICE_ID_TXT, "0", wxDefaultPosition, wxSize(40, TOP_BAR_COMP_HEIGHT));
    m_startStopButton = new wxButton(this, ID_ADD_BTN, "Start", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
    m_filterButton = new wxButton(this, ID_FILTER_BTN, "No filter set. Click a tree item to filter.", wxDefaultPosition, wxSize(-1, TOP_BAR_COMP_HEIGHT));
    m_filterButton->Enable(false);
    auto *clearButton = new wxButton(this, ID_CLEAR_BTN, "Clear", wxDefaultPosition, wxSize(-1, TOP_BAR_COMP_HEIGHT));
    m_logToFileCheckBox = new wxCheckBox(this, ID_LOG_TO_FILE_CHECKBOX, "Log to File"); 

    // --- Tree View Initialization ---
    m_milStd1553Tree = new wxTreeCtrl(this, ID_RT_SA_TREE, wxDefaultPosition, wxSize(250, -1), wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT); 
    auto rtSaTreeRoot = m_milStd1553Tree->AddRoot("MIL-STD-1553 Buses"); 
    for (size_t i = 0; i < MilStd1553::getInstance().busList.size(); ++i) {
        auto& bus = MilStd1553::getInstance().busList.at(i);
        bus.setTreeObject(m_milStd1553Tree->AppendItem(rtSaTreeRoot, bus.getName()));
        for (size_t j = 0; j < bus.rtList.size(); ++j) {
            auto& rt = bus.rtList.at(j);
            rt.setTreeObject(m_milStd1553Tree->AppendItem(bus.getTreeObject(), rt.getName()));
            for (size_t k = 0; k < rt.saList.size(); ++k) {
                auto& sa = rt.saList.at(k);
                sa.setTreeObject(m_milStd1553Tree->AppendItem(rt.getTreeObject(), sa.getName()));
            }
            wxTreeItemId mcRoot = m_milStd1553Tree->AppendItem(rt.getTreeObject(), "Mode Codes");
            for (const auto& mc_pair : MilStd1553::getModeCodeList()) {
                wxString mcLabel = wxString::Format("MC %d: %s", mc_pair.first, mc_pair.second);
                wxTreeItemId mcItem = m_milStd1553Tree->AppendItem(mcRoot, mcLabel);
                m_treeItemToMcMap[mcItem] = mc_pair.first;
            }
        }
        if (i == 0) m_milStd1553Tree->Expand(bus.getTreeObject()); 
    }

    // --- Message List (Log) Setup ---
    m_messageList = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_DONTWRAP);
    wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_messageList->SetFont(font);

    // --- Sizer Layout ---
    auto *topHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    topHorizontalSizer->Add(deviceIdText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
    topHorizontalSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_startStopButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_filterButton, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_logToFileCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5); 
    topHorizontalSizer->Add(clearButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    auto *bottomHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomHorizontalSizer->Add(m_milStd1553Tree, 0, wxEXPAND | wxALL, 5); 
    bottomHorizontalSizer->Add(m_messageList, 1, wxEXPAND | wxALL, 5);   
    auto *mainVerticalSizer = new wxBoxSizer(wxVERTICAL);
    mainVerticalSizer->Add(topHorizontalSizer, 0, wxEXPAND | wxALL, 5);
    mainVerticalSizer->Add(bottomHorizontalSizer, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(mainVerticalSizer);
    
    m_uiRecentMessageCount = 2000; // Varsayılan değer
    m_deviceIdTextInput->SetValue("0");

    // --- Olayları Bağlama ---
    m_startStopButton->Bind(wxEVT_BUTTON, &BMPanel::onStartStopClicked, this);
    m_filterButton->Bind(wxEVT_BUTTON, &BMPanel::onClearFilterClicked, this);
    clearButton->Bind(wxEVT_BUTTON, &BMPanel::onClearClicked, this);
    m_milStd1553Tree->Bind(wxEVT_TREE_ITEM_ACTIVATED, &BMPanel::onTreeItemClicked, this);
    m_logToFileCheckBox->Bind(wxEVT_CHECKBOX, &BMPanel::onLogToFileToggled, this);
}

BMPanel::~BMPanel() {}

void BMPanel::onStartStopClicked(wxCommandEvent &) {
    if (BM::getInstance().isMonitoring()) {
        m_mainFrame->SetStatusText("Stopping monitoring...");
        BM::getInstance().stop();
        m_startStopButton->SetLabelText("Start");
        m_deviceIdTextInput->Enable(true);
    } else {
        resetTreeVisualState();
        m_messageList->Clear();
        ConfigBmUi bmConfig;
        bmConfig.ulDevice = wxAtoi(m_deviceIdTextInput->GetValue());
        m_mainFrame->SetStatusText("Starting monitoring...");
        BM::getInstance().start(bmConfig);
        if(BM::getInstance().isMonitoring()){
            m_startStopButton->SetLabelText("Stop");
            m_deviceIdTextInput->Enable(false);
            m_mainFrame->SetStatusText("Monitoring started.");
        } else {
            m_mainFrame->SetStatusText("Failed to start monitoring.");
        }
    }
}

void BMPanel::appendMessagesToUi(const wxString& newMessagesChunk) { /* ... */ }
void BMPanel::updateTreeItemVisualState(char bus, int rt, int sa, bool isActive) { /* ... */ }
void BMPanel::resetTreeVisualState() { /* ... */ }
void BMPanel::onClearFilterClicked(wxCommandEvent &) { /* ... */ }
void BMPanel::onClearClicked(wxCommandEvent &) { /* ... */ }
void BMPanel::onTreeItemClicked(wxTreeEvent &event) { /* ... */ }
void BMPanel::onLogToFileToggled(wxCommandEvent &event) { /* ... */ }