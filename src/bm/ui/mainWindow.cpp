#include "mainWindow.hpp"
#include "bm.hpp"
#include "milStd1553.hpp"
#include <nlohmann/json.hpp> 
#include <fstream>
#include <string>
#include <wx/arrstr.h> 

wxBEGIN_EVENT_TABLE(BusMonitorFrame, wxFrame)
    EVT_MENU(ID_ADD_MENU, BusMonitorFrame::onStartStopClicked)
    EVT_BUTTON(ID_ADD_BTN, BusMonitorFrame::onStartStopClicked)
    EVT_MENU(ID_FILTER_MENU, BusMonitorFrame::onClearFilterClicked)
    EVT_BUTTON(ID_FILTER_BTN, BusMonitorFrame::onClearFilterClicked)
    EVT_MENU(ID_CLEAR_MENU, BusMonitorFrame::onClearClicked)
    EVT_BUTTON(ID_CLEAR_BTN, BusMonitorFrame::onClearClicked)
    EVT_BUTTON(ID_RESET_STREAM_BTN, BusMonitorFrame::onResetStreamClicked) 
    EVT_MENU(wxID_EXIT, BusMonitorFrame::onExit)
    EVT_TREE_ITEM_ACTIVATED(ID_RT_SA_TREE, BusMonitorFrame::onTreeItemClicked)
    EVT_CHECKBOX(ID_LOG_TO_FILE_CHECKBOX, BusMonitorFrame::onLogToFileToggled)
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
    m_deviceIdTextInput = new wxTextCtrl(this, ID_DEVICE_ID_TXT, "0", wxDefaultPosition, wxSize(40, -1));
    
    auto* streamChoiceText = new wxStaticText(this, wxID_ANY, "Stream:");
    wxArrayString streamChoices;
    m_totalStreams = 0;
    AiUInt32 outputCount = 0;

    AiReturn retVal = ApiInit();
    if (retVal > 0) {
        TY_API_OPEN tempOpen;
        AiUInt32 tempHandle;
        long deviceNumLong = 0;
        m_deviceIdTextInput->GetValue().ToLong(&deviceNumLong);
        tempOpen.ul_Module = static_cast<AiUInt32>(deviceNumLong);
        tempOpen.ul_Stream = 1;
        strcpy(tempOpen.ac_SrvName, "local");
        if (ApiOpenEx(&tempOpen, &tempHandle) == API_OK) {
            if (ApiCmdSysGetBoardInfo(tempHandle, TY_BOARD_INFO_CHANNEL_COUNT, 1, &m_totalStreams, &outputCount) == API_OK && outputCount > 0) {
                Logger::info("Bulunan stream sayisi: " + std::to_string(m_totalStreams));
                for (AiUInt32 i = 1; i <= m_totalStreams; ++i) {
                    streamChoices.Add(wxString::Format("Stream %u", i));
                }
            } else {
                 Logger::error("ApiCmdSysGetBoardInfo ile stream sayisi alinamadi.");
            }
            ApiClose(tempHandle);
        } else {
            Logger::error("Stream sayisini almak icin kart acilamadi.");
        }
    }
    if (streamChoices.IsEmpty()) {
        streamChoices.Add("Stream 1");
        m_totalStreams = 1;
        Logger::warn("Stream sayisi tespit edilemedi, varsayilan olarak 1 ayarlandi.");
    }
    m_streamChoiceComboBox = new wxComboBox(this, ID_STREAM_CHOICE_COMBOBOX, streamChoices[0], wxDefaultPosition, wxDefaultSize, streamChoices, wxCB_READONLY);
    
    m_startStopButton = new wxButton(this, ID_ADD_BTN, "Start", wxDefaultPosition, wxSize(100, -1));
    m_startStopButton->SetBackgroundColour(wxColour("#ffcc00"));
    m_resetStreamButton = new wxButton(this, ID_RESET_STREAM_BTN, "Reset Stream");
    m_filterButton = new wxButton(this, ID_FILTER_BTN, "No filter set. Click a tree item to filter.", wxDefaultPosition, wxSize(-1, -1));
    m_filterButton->Enable(false);
    auto *clearButton = new wxButton(this, ID_CLEAR_BTN, "Clear");
    m_logToFileCheckBox = new wxCheckBox(this, ID_LOG_TO_FILE_CHECKBOX, "Log to File"); 

    m_milStd1553Tree = new wxTreeCtrl(this, ID_RT_SA_TREE, wxDefaultPosition, wxSize(200, -1), wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT); 
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
    m_messageList = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_DONTWRAP);
    wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_messageList->SetFont(font);

    auto *topHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    topHorizontalSizer->Add(deviceIdText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_deviceIdTextInput, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(streamChoiceText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    topHorizontalSizer->Add(m_streamChoiceComboBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    topHorizontalSizer->Add(m_startStopButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_resetStreamButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_filterButton, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    topHorizontalSizer->Add(m_logToFileCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5); 
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
    int defaultDeviceNum = 0;
    std::string configPath = Common::getConfigPath();
    std::ifstream ifs(configPath);
    if (ifs.is_open()) {
        try {
            nlohmann::json configJson;
            ifs >> configJson;
            Logger::info("Successfully opened and parsed " + configPath);
            if (configJson.contains("Bus_Monitor")) {
                const auto& bmConfig = configJson["Bus_Monitor"];
                if (bmConfig.contains("Default_Device_Number")) {
                    defaultDeviceNum = bmConfig.value("Default_Device_Number", 0);
                    Logger::info("Loaded Default_Device_Number: " + std::to_string(defaultDeviceNum));
                }
                if (bmConfig.contains("UI_Recent_Line_Count")) {
                    m_uiRecentMessageCount = bmConfig.value("UI_Recent_Line_Count", 2000);
                    Logger::info("Loaded UI_Recent_Line_Count: " + std::to_string(m_uiRecentMessageCount));
                }
            }
        } catch (const nlohmann::json::parse_error &e) {
            Logger::error("JSON parse error in " + configPath + ": " + std::string(e.what()));
        }
    } else {
        Logger::info("Config file not found: " + configPath + ". Using defaults.");
    }
    m_deviceIdTextInput->SetValue(std::to_string(defaultDeviceNum));

    BM::getInstance().setUpdateMessagesCallback(
        [this](const std::string& messages) {
            wxTheApp->CallAfter([this, messages] {
                appendMessagesToUi(wxString::FromUTF8(messages.c_str()));
            });
        }
    );
    BM::getInstance().setUpdateTreeItemCallback(
        [this](char bus, int rt, int sa, bool isActive) {
            wxTheApp->CallAfter([this, bus, rt, sa, isActive] {
                updateTreeItemVisualState(bus, rt, sa, isActive);
            });
        }
    );
}

BusMonitorFrame::~BusMonitorFrame() {}

void BusMonitorFrame::appendMessagesToUi(const wxString& newMessagesChunk) {
    m_messageList->AppendText(newMessagesChunk);
    int lines = m_messageList->GetNumberOfLines();
    if (lines > m_uiRecentMessageCount) {
        int linesToRemove = lines - m_uiRecentMessageCount;
        long pos = m_messageList->XYToPosition(0, linesToRemove);
        if (pos > 0) {
            m_messageList->Remove(0, pos);
        }
    }
}

void BusMonitorFrame::updateTreeItemVisualState(char bus, int rt, int sa, bool isActive) {
    int bus_idx = (toupper(bus) == 'A') ? 0 : 1;
    if (bus_idx >= BUS_COUNT || rt >= RT_COUNT || sa >= SA_COUNT) return;

    auto& model = MilStd1553::getInstance();
    wxTreeItemId busTreeId = model.busList.at(bus_idx).getTreeObject();
    wxTreeItemId rtTreeId  = model.busList.at(bus_idx).rtList.at(rt).getTreeObject();
    wxTreeItemId saTreeId  = model.busList.at(bus_idx).rtList.at(rt).saList.at(sa).getTreeObject();

    if (isActive) {
        if (saTreeId.IsOk())  m_milStd1553Tree->SetItemTextColour(saTreeId, *wxGREEN);
        if (rtTreeId.IsOk())  m_milStd1553Tree->SetItemTextColour(rtTreeId, *wxGREEN);
        if (busTreeId.IsOk()) m_milStd1553Tree->SetItemTextColour(busTreeId, *wxGREEN);
        
        if (saTreeId.IsOk())  m_milStd1553Tree->SetItemBold(saTreeId, true);
        if (rtTreeId.IsOk())  m_milStd1553Tree->SetItemBold(rtTreeId, true);
        if (busTreeId.IsOk()) m_milStd1553Tree->SetItemBold(busTreeId, true);
    } 
}

void BusMonitorFrame::onStartStopClicked(wxCommandEvent &) {
    if (BM::getInstance().isMonitoring()) {
        SetStatusText("Stopping monitoring...");
        BM::getInstance().stop();
        m_startStopButton->SetLabelText("Start");
        m_startStopButton->SetBackgroundColour(wxColour("#ffcc00"));
        SetStatusText("Monitoring stopped. Ready to start.");
        m_deviceIdTextInput->Enable(true);
        m_streamChoiceComboBox->Enable(true);
        wxCommandEvent emptyEvent;
        onClearFilterClicked(emptyEvent);
    } else {
        long deviceNumLong = -1;
        if (!m_deviceIdTextInput->GetValue().ToLong(&deviceNumLong) || deviceNumLong < 0) {
            wxMessageBox("Invalid Device ID. Please enter a non-negative integer.", "Error", wxOK | wxICON_ERROR, this);
            return;
        }

        AiUInt32 openConnections = 0;
        BM::getInstance().checkDeviceInUse(static_cast<AiUInt32>(deviceNumLong), openConnections);
        if (openConnections > 0) {
            wxString msg;
            msg.Printf("Warning: Device %ld might be in use by another process (%u open handles). Continue?", deviceNumLong, openConnections);
            if (wxMessageBox(msg, "Device In Use", wxYES_NO | wxICON_WARNING, this) != wxYES) {
                return;
            }
        }
        
        bool shouldLogData = m_logToFileCheckBox->IsChecked();
        BM::getInstance().enableDataLogging(shouldLogData);
        if (shouldLogData) {
            Logger::info("Monitoring started with data logging ENABLED.");
        }

        resetTreeVisualState();
        m_messageList->Clear();

        ConfigBmUi bmConfig;
        bmConfig.ulDevice = static_cast<AiUInt32>(deviceNumLong);
        
        int selectedIndex = m_streamChoiceComboBox->GetSelection();
        bmConfig.ulStream = static_cast<AiUInt32>(selectedIndex + 1);
        
        bmConfig.ulCoupling = API_CAL_CPL_TRANSFORM;
        
        wxString statusMsg;
        statusMsg.Printf("Starting monitoring on device %ld, stream %u...", deviceNumLong, bmConfig.ulStream);
        SetStatusText(statusMsg);
        
        AiReturn bmStartRet = BM::getInstance().start(bmConfig);

        if (bmStartRet == API_OK) {
            statusMsg.Printf("Monitoring started on device %ld, stream %u", deviceNumLong, bmConfig.ulStream);
            SetStatusText(statusMsg);
            m_startStopButton->SetLabelText("Stop");
            m_startStopButton->SetBackgroundColour(wxColour("#ff4545"));
            m_startStopButton->SetForegroundColour(wxColour("white"));
            m_deviceIdTextInput->Enable(false);
            m_streamChoiceComboBox->Enable(false);
        } else {
            std::string errorString = getAIMApiErrorMessage(bmStartRet);
            SetStatusText(("Error starting: " + errorString).c_str());
            wxMessageBox("Failed to start Bus Monitor: " + errorString, "Error", wxOK | wxICON_ERROR, this);
            m_deviceIdTextInput->Enable(true);
            m_streamChoiceComboBox->Enable(true);
        }
    }
}

void BusMonitorFrame::onClearFilterClicked(wxCommandEvent &) {
    if (!BM::getInstance().isFilterEnabled()) return;
    BM::getInstance().enableFilter(false);
    m_filterButton->SetLabelText("No filter set. Click a tree item to filter.");
    m_filterButton->Enable(false);
    resetTreeVisualState();
    SetStatusText("Filter cleared.");
}

void BusMonitorFrame::onClearClicked(wxCommandEvent &) {
    m_messageList->Clear();
    resetTreeVisualState();
    SetStatusText("Messages cleared.");
}

void BusMonitorFrame::onTreeItemClicked(wxTreeEvent &event) {
    wxTreeItemId clickedId = event.GetItem();
    if (!clickedId.IsOk()) return;
    char filterBusChar = 0;
    int filterRt = -1;
    int filterSa = -1;
    int filterMc = -1;
    bool found = false;
    auto& model = MilStd1553::getInstance();
    auto it = m_treeItemToMcMap.find(clickedId);
    if (it != m_treeItemToMcMap.end()) {
        filterMc = it->second; 
        wxTreeItemId rtId = m_milStd1553Tree->GetItemParent(m_milStd1553Tree->GetItemParent(clickedId));
        wxTreeItemId busId = m_milStd1553Tree->GetItemParent(rtId);
        for(int i = 0; i < BUS_COUNT; ++i) {
            if (model.busList.at(i).getTreeObject() == busId) {
                filterBusChar = (i == 0) ? 'A' : 'B';
                for (int j = 0; j < RT_COUNT; ++j) {
                    if (model.busList.at(i).rtList.at(j).getTreeObject() == rtId) {
                        filterRt = j;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
        }
    } else {
        for (int i = 0; i < BUS_COUNT && !found; ++i) {
            auto& bus = model.busList.at(i);
            if (bus.getTreeObject() == clickedId) { filterBusChar = (i == 0) ? 'A' : 'B'; found = true; break; }
            for (size_t j = 0; j < bus.rtList.size() && !found; ++j) {
                auto& rt = bus.rtList.at(j);
                if (rt.getTreeObject() == clickedId) { filterBusChar = (i == 0) ? 'A' : 'B'; filterRt = j; found = true; break; }
                for (size_t k = 0; k < rt.saList.size() && !found; ++k) {
                    auto& sa = rt.saList.at(k);
                    if (sa.getTreeObject() == clickedId) { filterBusChar = (i == 0) ? 'A' : 'B'; filterRt = j; filterSa = k; found = true; break; }
                }
            }
        }
    }
    if (found) {
        BM::getInstance().setFilterCriteria(filterBusChar, filterRt, filterSa, filterMc);
        BM::getInstance().enableFilter(true);
        wxString filterLabel = "Filtering by: ";
        if(filterBusChar != 0) filterLabel += wxString::Format("Bus %c", filterBusChar);
        if(filterRt != -1) filterLabel += wxString::Format(", RT %d", filterRt);
        if(filterSa != -1) filterLabel += wxString::Format(", SA %d", filterSa);
        if(filterMc != -1) filterLabel += wxString::Format(", MC %d", filterMc);
        m_filterButton->SetLabelText(filterLabel);
        m_filterButton->Enable(true);
        resetTreeVisualState();
        m_milStd1553Tree->SetItemBold(clickedId, true);
        m_milStd1553Tree->EnsureVisible(clickedId);
        SetStatusText(filterLabel);
    }
}

void BusMonitorFrame::resetTreeVisualState() {
    auto& model = MilStd1553::getInstance();
    wxColour defaultColour = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    for (const auto& bus : model.busList) {
        if(bus.getTreeObject().IsOk()) { 
            m_milStd1553Tree->SetItemBold(bus.getTreeObject(), false); 
            m_milStd1553Tree->SetItemTextColour(bus.getTreeObject(), defaultColour); 
        }
        for (const auto& rt : bus.rtList) {
            if(rt.getTreeObject().IsOk()) { 
                m_milStd1553Tree->SetItemBold(rt.getTreeObject(), false); 
                m_milStd1553Tree->SetItemTextColour(rt.getTreeObject(), defaultColour); 
            }
            for (const auto& sa : rt.saList) {
                if(sa.getTreeObject().IsOk()) { 
                    m_milStd1553Tree->SetItemBold(sa.getTreeObject(), false); 
                    m_milStd1553Tree->SetItemTextColour(sa.getTreeObject(), defaultColour); 
                }
            }
        }
    }
}

void BusMonitorFrame::onLogToFileToggled(wxCommandEvent &event) {
    bool isChecked = event.IsChecked();
    BM::getInstance().enableDataLogging(isChecked);
    if (isChecked) {
        SetStatusText("Data logging to file enabled.");
        Logger::info("Data logging to file ENABLED by user.");
    } else {
        SetStatusText("Data logging to file disabled.");
        Logger::info("Data logging to file DISABLED by user.");
    }
}

void BusMonitorFrame::onExit(wxCommandEvent &) { Close(true); }

void BusMonitorFrame::onCloseFrame(wxCloseEvent&) {
    if (BM::getInstance().isMonitoring()) {
        BM::getInstance().stop();
    }
    Destroy();
}

void BusMonitorFrame::onResetStreamClicked(wxCommandEvent &event) {
    if (BM::getInstance().isMonitoring()) {
        wxMessageBox("Please stop the monitor before resetting a stream.", "Warning", wxOK | wxICON_WARNING, this);
        return;
    }

    long deviceNumLong = -1;
    m_deviceIdTextInput->GetValue().ToLong(&deviceNumLong);

    int selectedIndex = m_streamChoiceComboBox->GetSelection();
    if (selectedIndex == wxNOT_FOUND) return;
    
    int streamToReset = selectedIndex + 1;

    wxString msg;
    msg.Printf("Are you sure you want to reset Stream %d on Device %ld?\nThis will clear its configuration.", streamToReset, deviceNumLong);
    if (wxMessageBox(msg, "Confirm Reset", wxYES_NO | wxICON_QUESTION, this) != wxYES) {
        return;
    }

    AiReturn ret = BM::getInstance().resetStream(static_cast<AiUInt32>(deviceNumLong), streamToReset);

    if (ret == API_OK) {
        SetStatusText(wxString::Format("Stream %d successfully reset.", streamToReset));
    } else {
        std::string errorString = getAIMApiErrorMessage(ret);
        SetStatusText(wxString::Format("Failed to reset Stream %d: %s", streamToReset, errorString.c_str()));
    }
}