#include "mainWindow.hpp"
#include "bm.hpp"
#include "milStd1553.hpp"
#include <nlohmann/json.hpp> 
#include <fstream>
#include <string>
#include <wx/arrstr.h> 


// Event table for connecting UI events to their handler functions.
// This is a classic wxWidgets approach for event handling.
wxBEGIN_EVENT_TABLE(BusMonitorFrame, wxFrame)
    EVT_MENU(ID_ADD_MENU, BusMonitorFrame::onStartStopClicked)
    EVT_BUTTON(ID_ADD_BTN, BusMonitorFrame::onStartStopClicked)
    EVT_MENU(ID_FILTER_MENU, BusMonitorFrame::onClearFilterClicked)
    EVT_BUTTON(ID_FILTER_BTN, BusMonitorFrame::onClearFilterClicked)
    EVT_MENU(ID_CLEAR_MENU, BusMonitorFrame::onClearClicked)
    EVT_BUTTON(ID_CLEAR_BTN, BusMonitorFrame::onClearClicked)
    EVT_MENU(wxID_EXIT, BusMonitorFrame::onExit)
    EVT_TREE_ITEM_ACTIVATED(ID_RT_SA_TREE, BusMonitorFrame::onTreeItemClicked)
    EVT_CHECKBOX(ID_LOG_TO_FILE_CHECKBOX, BusMonitorFrame::onLogToFileToggled)
    EVT_CLOSE(BusMonitorFrame::onCloseFrame)
wxEND_EVENT_TABLE()

/**
 * @brief Constructor for the main application frame.
 *        Initializes all UI components, sets up layout, loads configuration,
 *        and establishes communication with the backend BM singleton.
 */
BusMonitorFrame::BusMonitorFrame() : wxFrame(nullptr, wxID_ANY, "MIL-STD-1553 Bus Monitor") {
    // 1. --- UI Component Creation and Layout ---
    // This section follows the standard wxWidgets pattern: create controls,
    // arrange them in sizers, and then set the top-level sizer for the frame.
    // std::cout << "--- PATH DEBUGGING ---" << std::endl;
    // std::cout << "Executable Dir: " << Common::getExecutableDirectory() << std::endl;
    // std::cout << "Project Root:   " << Common::getProjectRootDirectory() << std::endl;
    // std::cout << "Config Path:    " << Common::getConfigPath() << std::endl;
    // std::cout << "----------------------" << std::endl;
    // --- Menu Bar Setup ---
    auto *menuFile = new wxMenu;
    menuFile->Append(ID_ADD_MENU, "Start / Stop\tCtrl-R", "Start or stop monitoring");
    menuFile->Append(ID_FILTER_MENU, "Clear filter\tCtrl-F", "Clear filtering of messages");
    menuFile->Append(ID_CLEAR_MENU, "Clear messages\tCtrl-M", "Clear messages");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    auto *menuBar = new wxMenuBar;
    SetMenuBar(menuBar);
    menuBar->Append(menuFile, "&Commands");

    // --- Top Control Bar ---
    auto *deviceIdText = new wxStaticText(this, wxID_ANY, "AIM Device ID:");
    m_deviceIdTextInput = new wxTextCtrl(this, ID_DEVICE_ID_TXT, "0", wxDefaultPosition, wxSize(40, TOP_BAR_COMP_HEIGHT));
    m_startStopButton = new wxButton(this, ID_ADD_BTN, "Start", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
    m_startStopButton->SetBackgroundColour(wxColour("#ffcc00"));
    m_filterButton = new wxButton(this, ID_FILTER_BTN, "No filter set. Click a tree item to filter.", wxDefaultPosition, wxSize(-1, TOP_BAR_COMP_HEIGHT));
    m_filterButton->Enable(false);
    auto *clearButton = new wxButton(this, ID_CLEAR_BTN, "Clear", wxDefaultPosition, wxSize(-1, TOP_BAR_COMP_HEIGHT));
    m_logToFileCheckBox = new wxCheckBox(this, ID_LOG_TO_FILE_CHECKBOX, "Log to File"); 

    // --- Tree View Initialization ---
    // The tree is pre-populated from the MilStd1553 data model.
    // Each visual item's wxTreeItemId is stored back into the model,
    // allowing for efficient reverse lookups later (finding the model item from a clicked tree item).
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

    SetSizer(mainVerticalSizer);
    SetMinSize(wxSize(800, 600)); 
    Centre();
    CreateStatusBar();
    SetStatusText("Ready, press Start");

// In src/bm/ui/mainWindow.cpp

        // 2. --- Configuration Loading ---
        m_uiRecentMessageCount = 2000; // Start with a default
        int defaultDeviceNum = 0;      // Start with a default

        std::string configPath = Common::getConfigPath();
        std::ifstream ifs(configPath);

        if (ifs.is_open()) {
            try {
                nlohmann::json configJson;
                ifs >> configJson;
                Logger::info("Successfully opened and parsed " + configPath);

                // Check for and read Bus Monitor settings
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

        // Now, apply the loaded (or default) value to the UI text input
        m_deviceIdTextInput->SetValue(std::to_string(defaultDeviceNum));
    // ...
    
    // 3. --- Backend Communication Setup ---
    // Sets up callback functions to receive data from the BM backend.
    // CRITICAL: Uses wxTheApp->CallAfter to safely marshal calls from the BM's
    // worker thread to the main UI thread, preventing race conditions and crashes.

    /**
    * @brief Callback to receive formatted message strings from the backend.
    * 
    * This lambda is passed to the BM singleton. When the backend has new data,
    * it invokes this callback. The call is marshaled to the main UI thread
    * via wxTheApp->CallAfter to safely update the message list.
    */
    BM::getInstance().setUpdateMessagesCallback(
        [this](const std::string& messages) {
            wxTheApp->CallAfter([this, messages] {
                appendMessagesToUi(wxString::FromUTF8(messages.c_str()));
            });
        }
    );
// In BusMonitorFrame::BusMonitorFrame()

// ...

// 3. --- Backend Communication Setup ---
// Sets up callback functions to receive data from the BM backend.
// CRITICAL: Uses wxTheApp->CallAfter to safely marshal calls from the BM's
// worker thread to the main UI thread, preventing race conditions and crashes.

/**
 * @brief Callback to receive formatted message strings from the backend.
 * 
 * This lambda is passed to the BM singleton. When the backend has new data,
 * it invokes this callback. The call is marshaled to the main UI thread
 * via wxTheApp->CallAfter to safely update the message list.
 */
 BM::getInstance().setUpdateMessagesCallback(
    [this](const std::string& messages) {
        wxTheApp->CallAfter([this, messages] {
            appendMessagesToUi(wxString::FromUTF8(messages.c_str()));
        });
    }
);

    /**
    * @brief Callback to receive active terminal information for visual updates.
    * 
    * This lambda is invoked by the backend whenever a new terminal becomes active.
    * It passes the bus/RT/SA coordinates, which are then used to highlight the
    * corresponding item in the UI's tree view, again safely via wxTheApp->CallAfter.
    */
    BM::getInstance().setUpdateTreeItemCallback(
        [this](char bus, int rt, int sa, bool isActive) {
            wxTheApp->CallAfter([this, bus, rt, sa, isActive] {
                updateTreeItemVisualState(bus, rt, sa, isActive);
            });
        }
    );
}

/**
 * @brief Destructor for the main frame.
 *        No special cleanup is needed here as child windows are managed by wxWidgets.
 */
BusMonitorFrame::~BusMonitorFrame() {}

/**
 * @brief Appends a new chunk of messages to the UI's message list.
 *        To prevent performance degradation from an infinitely growing text control,
 *        this function trims the oldest lines if the total line count exceeds
 *        the configured `m_uiRecentMessageCount`.
 * @param newMessagesChunk The new block of formatted text to add.
 */
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

/**
 * @brief Updates the visual state of a tree item based on bus activity.
 *        This function is the end-point for the UI update callback. It changes the
 *        color and style of the corresponding Bus, RT, and SA items to provide
 *        real-time feedback to the user.
 * @param bus The bus ('A' or 'B') of the active item.
 * @param rt The RT address (0-31).
 * @param sa The Subaddress (0-31).
 * @param isActive If true, the item is highlighted; otherwise, no action is taken.
 */
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

/**
 * @brief Event handler for the Start/Stop button and menu item.
 *        Toggles the monitoring state of the BM backend and updates the UI accordingly.
 */
void BusMonitorFrame::onStartStopClicked(wxCommandEvent &) {
    if (BM::getInstance().isMonitoring()) {
        SetStatusText("Stopping monitoring...");
        BM::getInstance().stop();
        m_startStopButton->SetLabelText("Start");
        m_startStopButton->SetBackgroundColour(wxColour("#ffcc00"));
        SetStatusText("Monitoring stopped. Ready to start.");
        m_deviceIdTextInput->Enable(true);
        wxCommandEvent emptyEvent;
        onClearFilterClicked(emptyEvent);
    } else {
        long deviceNumLong = -1;
        if (!m_deviceIdTextInput->GetValue().ToLong(&deviceNumLong) || deviceNumLong < 0) {
            wxMessageBox("Invalid Device ID. Please enter a non-negative integer.", "Error", wxOK | wxICON_ERROR, this);
            return;
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

/**
 * @brief Event handler for the "Clear Filter" button and menu item.
 *        Disables filtering in the backend and resets the UI filter button to its default state.
 */
void BusMonitorFrame::onClearFilterClicked(wxCommandEvent &) {
    if (!BM::getInstance().isFilterEnabled()) return;
    BM::getInstance().enableFilter(false);
    m_filterButton->SetLabelText("No filter set. Click a tree item to filter.");
    m_filterButton->Enable(false);
    resetTreeVisualState();
    SetStatusText("Filter cleared.");
}

/**
 * @brief Event handler for the "Clear" button and menu item.
 *        Clears the message list and resets any visual state in the tree.
 */
void BusMonitorFrame::onClearClicked(wxCommandEvent &) {
    m_messageList->Clear();
    resetTreeVisualState();
    SetStatusText("Messages cleared.");
}

/**
 * @brief Event handler for a double-click or Enter press on a tree item.
 *        This function performs a "reverse lookup" to identify which Bus/RT/SA was
 *        clicked and sets the backend filter criteria accordingly.
 * @param event The wxTreeEvent containing the ID of the clicked item.
 */
void BusMonitorFrame::onTreeItemClicked(wxTreeEvent &event) {
    wxTreeItemId clickedId = event.GetItem();
    if (!clickedId.IsOk()) return;

    char filterBusChar = 0;
    int filterRt = -1;
    int filterSa = -1;
    int filterMc = -1;
    bool found = false;
    auto& model = MilStd1553::getInstance();
    // 1. Tıklanan öğenin bir Mode Code olup olmadığını kontrol et
    auto it = m_treeItemToMcMap.find(clickedId);
    if (it != m_treeItemToMcMap.end()) {
        filterMc = it->second; // MC numarasını al
        // Parent'larına giderek RT ve Bus bilgilerini bul
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
        // 2. Eğer Mode Code değilse, eski Bus/RT/SA arama mantığını kullan
        for (int i = 0; i < BUS_COUNT && !found; ++i) {
            auto& bus = model.busList.at(i);
            if (bus.getTreeObject() == clickedId) { filterBusChar = (i == 0) ? 'A' : 'B'; found = true; break; }
            for (int j = 0; j < bus.rtList.size() && !found; ++j) {
                auto& rt = bus.rtList.at(j);
                if (rt.getTreeObject() == clickedId) { filterBusChar = (i == 0) ? 'A' : 'B'; filterRt = j; found = true; break; }
                for (int k = 0; k < rt.saList.size() && !found; ++k) {
                    auto& sa = rt.saList.at(k);
                    if (sa.getTreeObject() == clickedId) { filterBusChar = (i == 0) ? 'A' : 'B'; filterRt = j; filterSa = k; found = true; break; }
                }
            }
        }
    }

    if (found) {
        // Backend'i yeni kriterlerle ayarla
        BM::getInstance().setFilterCriteria(filterBusChar, filterRt, filterSa, filterMc);
        BM::getInstance().enableFilter(true);

        // UI'daki filtre etiketini güncelle
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


/**
 * @brief Resets the visual state of all items in the tree control to default.
 *        This is called when monitoring starts, stops, or when filters/logs are cleared
 *        to ensure a consistent UI state.
 */
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

/**
 * @brief Enables or disables logging of bus data to a file based on checkbox state.
 */
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

/**
 * @brief Event handler for the Exit menu item.
 */
void BusMonitorFrame::onExit(wxCommandEvent &) { Close(true); }

/**
 * @brief Event handler for the window close event (e.g., clicking the 'X' button).
 *        Ensures that the backend monitoring is stopped cleanly before the application exits.
 */
void BusMonitorFrame::onCloseFrame(wxCloseEvent&) {
    if (BM::getInstance().isMonitoring()) {
        BM::getInstance().stop();
    }
    Destroy();
}