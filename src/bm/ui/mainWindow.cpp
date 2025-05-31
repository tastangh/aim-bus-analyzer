#include "mainWindow.hpp"
#include "bm.hpp"
#include "milStd1553.hpp"
#include <nlohmann/json.hpp> 
#include <fstream>
#include <string>
#include <regex>   
#include <wx/arrstr.h> 
#include <wx/tokenzr.h> 


#ifndef CONFIG_PATH
#define CONFIG_PATH "config.json" 
#endif

namespace Logger { 
    void error(const std::string& msg) { wxLogError("%s", msg.c_str()); }
    void info(const std::string& msg) { wxLogMessage("%s", msg.c_str()); }
}


wxBEGIN_EVENT_TABLE(BusMonitorFrame, wxFrame)
    EVT_MENU(ID_ADD_MENU, BusMonitorFrame::onStartStopClicked)
    EVT_BUTTON(ID_ADD_BTN, BusMonitorFrame::onStartStopClicked)
    EVT_MENU(ID_FILTER_MENU, BusMonitorFrame::onClearFilterClicked)
    EVT_BUTTON(ID_FILTER_BTN, BusMonitorFrame::onClearFilterClicked)
    EVT_MENU(ID_CLEAR_MENU, BusMonitorFrame::onClearClicked)
    EVT_BUTTON(ID_CLEAR_BTN, BusMonitorFrame::onClearClicked)
    EVT_MENU(wxID_EXIT, BusMonitorFrame::onExit)
    EVT_TREE_ITEM_ACTIVATED(ID_RT_SA_TREE, BusMonitorFrame::onTreeItemClicked)
    EVT_CLOSE(BusMonitorFrame::onCloseFrame)
wxEND_EVENT_TABLE()


BusMonitorFrame::BusMonitorFrame() : wxFrame(nullptr, wxID_ANY, "MIL-STD-1553 Bus Monitor") {
    auto *menuFile = new wxMenu;
    menuFile->Append(ID_ADD_MENU, "Start / Stop\tCtrl-R", "Start or stop monitoring");
    menuFile->Append(ID_FILTER_MENU, "Clear filter\tCtrl-F", "Clear filtering of messages");
    menuFile->Append(ID_CLEAR_MENU, "Clear messages\tCtrl-M", "Clear messages");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto *menuBar = new wxMenuBar;
    SetMenuBar(menuBar);
    menuBar->Append(menuFile, "&Commands");

    auto *deviceIdText = new wxStaticText(this, wxID_ANY, "AIM Device ID:");

    m_deviceIdTextInput = new wxTextCtrl(
        this, ID_DEVICE_ID_TXT, "0", wxDefaultPosition,
        wxSize(40, TOP_BAR_COMP_HEIGHT));

    m_startStopButton = new wxButton(
        this, ID_ADD_BTN, "Start", wxDefaultPosition,
        wxSize(100, TOP_BAR_COMP_HEIGHT));
    m_startStopButton->SetBackgroundColour(wxColour("#ffcc00"));
    m_startStopButton->SetForegroundColour(
        wxColour(wxSystemSettings::GetAppearance().IsDark() ? "black" : wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)));


    m_filterButton = new wxButton(
        this, ID_FILTER_BTN, "No filter set, displaying all messages.", wxDefaultPosition,
        wxSize(-1, TOP_BAR_COMP_HEIGHT));
    m_filterButton->Enable(false);

    auto *clearButton = new wxButton(
        this, ID_CLEAR_BTN, "Clear", wxDefaultPosition,
        wxSize(-1, TOP_BAR_COMP_HEIGHT));

    m_milStd1553Tree =
        new wxTreeCtrl(this, ID_RT_SA_TREE, wxDefaultPosition,
                        wxSize(200, -1), wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT); 

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
        }
        if (i == 0) m_milStd1553Tree->Expand(bus.getTreeObject()); 
    }


    m_messageList = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                    wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_DONTWRAP);
    wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_messageList->SetFont(font);


    auto *topHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    topHorizontalSizer->Add(deviceIdText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
    topHorizontalSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_startStopButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_filterButton, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5); 
    topHorizontalSizer->Add(clearButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    auto *bottomHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomHorizontalSizer->Add(m_milStd1553Tree, 0, wxEXPAND | wxALL, 5); 
    bottomHorizontalSizer->Add(m_messageList, 1, wxEXPAND | wxALL, 5);   
    auto *mainVerticalSizer = new wxBoxSizer(wxVERTICAL);
    mainVerticalSizer->Add(topHorizontalSizer, 0, wxEXPAND | wxALL, 5);
    mainVerticalSizer->Add(bottomHorizontalSizer, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainVerticalSizer);
    SetMinSize(wxSize(800, 600)); 
    Centre();


    CreateStatusBar();
    SetStatusText("Ready, press Start");

    m_uiRecentMessageCount = 2000; 
    nlohmann::json configJson;
    std::ifstream ifs(CONFIG_PATH);
    if (ifs.is_open()) {
        try {
            ifs >> configJson;
            if (configJson.contains("Bus_Monitor")) {
                if (configJson["Bus_Monitor"].contains("Default_Device_Number") &&
                    configJson["Bus_Monitor"]["Default_Device_Number"].is_number_integer()) {
                    m_deviceIdTextInput->SetValue(std::to_string(configJson["Bus_Monitor"]["Default_Device_Number"].get<int>()));
                }
                if (configJson["Bus_Monitor"].contains("UI_Recent_Line_Count") &&
                    configJson["Bus_Monitor"]["UI_Recent_Line_Count"].is_number_integer()) {
                    m_uiRecentMessageCount = configJson["Bus_Monitor"]["UI_Recent_Line_Count"].get<int>();
                    if (m_uiRecentMessageCount <= 0) m_uiRecentMessageCount = 2000;
                }
            }
        } catch (const nlohmann::json::parse_error &e) {
            Logger::error("JSON parse error in " + std::string(CONFIG_PATH) + ": " + std::string(e.what()));
        }
        ifs.close();
    } else {
        Logger::info("Config file not found: " + std::string(CONFIG_PATH) + ". Using defaults.");
    }

    BM::getInstance().setUpdateMessagesCallback(
        [this](const std::string& messages) {
            if (wxIsMainThread()) {
                 appendMessagesToUi(wxString::FromUTF8(messages.c_str()));
            } else {
                wxTheApp->CallAfter([this, messages] {
                    appendMessagesToUi(wxString::FromUTF8(messages.c_str()));
                });
            }
        }
    );
}

BusMonitorFrame::~BusMonitorFrame() {
}

void BusMonitorFrame::appendMessagesToUi(const wxString& newMessagesChunk) {
    m_messageList->AppendText(newMessagesChunk);

    int lines = m_messageList->GetNumberOfLines();
    if (lines > m_uiRecentMessageCount) {
        int linesToRemove = lines - m_uiRecentMessageCount;
        long pos = 0;
        for (int i = 0; i < linesToRemove; ++i) {
            pos = m_messageList->XYToPosition(0, i + 1);
            if (pos == -1 && i < linesToRemove -1) { 
                 pos = m_messageList->XYToPosition(0,i+2);
                 if(pos == -1) {
                     Logger::error("Could not determine position to remove lines.");
                     return;
                 }
            } else if (pos == -1 && i == linesToRemove -1) { 
                pos = m_messageList->GetLastPosition(); 
            }
        }
         if (pos > 0) m_messageList->Remove(0, pos);
    }
}

void BusMonitorFrame::updateTreeItemVisualState(char bus, int rt, int sa, bool isActive) {
    // Bu fonksiyon implemente edilecek
}

void BusMonitorFrame::onStartStopClicked(wxCommandEvent & /*event*/) {
    if (BM::getInstance().isMonitoring()) {
        SetStatusText("Stopping monitoring...");
        BM::getInstance().stop();
        m_startStopButton->SetLabelText("Start");
        m_startStopButton->SetBackgroundColour(wxColour("#ffcc00"));
        m_startStopButton->SetForegroundColour(
             wxSystemSettings::GetColour(wxSystemSettingsNative::GetAppearance().IsDark() ? wxSYS_COLOUR_BTNTEXT : wxSYS_COLOUR_BTNTEXT));
        SetStatusText("Monitoring stopped. Ready to start.");
        m_deviceIdTextInput->Enable(true);
    } else {
        long deviceNumLong = -1;
        if (!m_deviceIdTextInput->GetValue().ToLong(&deviceNumLong) || deviceNumLong < 0) {
            wxMessageBox("Invalid Device ID. Please enter a non-negative integer.", "Error", wxOK | wxICON_ERROR, this);
            SetStatusText("Error: Invalid Device ID");
            return;
        }

        ConfigBmUi bmConfig;
        bmConfig.ulDevice = static_cast<AiUInt32>(deviceNumLong);
        bmConfig.ulStream = 1; 
        bmConfig.ulCoupling = API_CAL_CPL_TRANSFORM; 

        SetStatusText("Starting monitoring on device " + m_deviceIdTextInput->GetValue() + "...");
        AiReturn bmStartRet = BM::getInstance().start(bmConfig);

        if (bmStartRet == API_OK) {
            SetStatusText("Monitoring started on device " + m_deviceIdTextInput->GetValue());
            m_startStopButton->SetLabelText("Stop");
            m_startStopButton->SetBackgroundColour(wxColour("#ff4545"));
            m_startStopButton->SetForegroundColour(wxColour("white"));
            m_deviceIdTextInput->Enable(false);
        } else {
            std::string errorString = getAIMApiErrorMessage(bmStartRet);
            SetStatusText(("Error starting: " + errorString).c_str());
            wxMessageBox("Failed to start Bus Monitor: " + errorString, "Error", wxOK | wxICON_ERROR, this);
            m_deviceIdTextInput->Enable(true);
        }
    }
}

void BusMonitorFrame::onClearFilterClicked(wxCommandEvent & /*event*/) {
    if (!BM::getInstance().isFilterEnabled()) return;
    BM::getInstance().enableFilter(false);
    m_filterButton->SetLabelText("No filter set, displaying all messages.");
    m_filterButton->Enable(false);
    m_filterButton->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    SetStatusText("Filter cleared.");
}

void BusMonitorFrame::onClearClicked(wxCommandEvent & /*event*/) {
    m_messageList->Clear();
    SetStatusText("Messages cleared.");
}

void BusMonitorFrame::onTreeItemClicked(wxTreeEvent &event) {
    // filtreleme mantığı buraya gelecek
    wxLogMessage("Tree item clicked: %s", m_milStd1553Tree->GetItemText(event.GetItem()));
}

void BusMonitorFrame::onExit(wxCommandEvent & /*event*/) {
    Close(true);
}

void BusMonitorFrame::onCloseFrame(wxCloseEvent& event) {
    if (BM::getInstance().isMonitoring()) {
        BM::getInstance().stop();
    }
    event.Skip();
}