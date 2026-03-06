#include "DeviceSelectionPanel.hpp"
#include "MainFrame.hpp"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>

// Constructor ve diğer fonksiyonlar aynı kalır...
DeviceSelectionPanel::DeviceSelectionPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddStretchSpacer();
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Detected AIM Devices"), 0, wxALIGN_CENTER | wxBOTTOM, 10);
    
    m_deviceList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(400, 150));
    mainSizer->Add(m_deviceList, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    m_scanButton = new wxButton(this, wxID_ANY, "Scan for Hardware");
    mainSizer->Add(m_scanButton, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    auto* gridSizer = new wxFlexGridSizer(2, 2, 10, 10);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Bus Controller Stream:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    m_bcStreamChoice = new wxChoice(this, wxID_ANY);
    gridSizer->Add(m_bcStreamChoice, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Bus Monitor Stream:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    m_bmStreamChoice = new wxChoice(this, wxID_ANY);
    gridSizer->Add(m_bmStreamChoice, 1, wxEXPAND);
    mainSizer->Add(gridSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    m_initializeButton = new wxButton(this, wxID_ANY, "Configure and Start");
    m_initializeButton->Enable(false);
    mainSizer->Add(m_initializeButton, 0, wxALIGN_CENTER);
    mainSizer->AddStretchSpacer();

    SetSizer(mainSizer);
    GetSizer()->Fit(this);

    m_scanButton->Bind(wxEVT_BUTTON, &DeviceSelectionPanel::onScanHardware, this);
    m_initializeButton->Bind(wxEVT_BUTTON, &DeviceSelectionPanel::onInitialize, this);
    m_deviceList->Bind(wxEVT_LISTBOX, [this](wxCommandEvent&){
        int sel = m_deviceList->GetSelection();
        if (sel != wxNOT_FOUND) {
            populateStreamChoices(m_bcStreamChoice, m_foundDevices[sel].streamCount);
            populateStreamChoices(m_bmStreamChoice, m_foundDevices[sel].streamCount);
            m_initializeButton->Enable(true);
        }
    });
}


// onScanHardware fonksiyonunu aşağıdakiyle tamamen değiştirin:
void DeviceSelectionPanel::onScanHardware(wxCommandEvent& event) {
    m_deviceList->Clear();
    m_foundDevices.clear();
    m_initializeButton->Enable(false);
    
    // Tıpkı test_aim.cpp gibi, API'yi burada başlatıyoruz.
    AiReturn ret = ApiInit();
    if (ret <= 0) {
        wxMessageBox(wxString::Format("ApiInit() failed with code %d. Is the driver running?", ret), "API Error", wxOK | wxICON_ERROR);
        return;
    }

    // Basit bir tarama - ilk 4 cihaz ID'sini kontrol et
    for (AiUInt32 devId = 0; devId < 4; ++devId) {
        TY_API_OPEN tempOpen;
        AiUInt32 tempHandle;
        memset(&tempOpen, 0, sizeof(tempOpen));
        tempOpen.ul_Module = devId;
        tempOpen.ul_Stream = 1;
        strcpy(tempOpen.ac_SrvName, "local");

        if (ApiOpenEx(&tempOpen, &tempHandle) == API_OK) {
            AiUInt32 streamCount = 0;
            AiUInt32 outputCount = 0;
            if (ApiCmdSysGetBoardInfo(tempHandle, TY_BOARD_INFO_CHANNEL_COUNT, 1, &streamCount, &outputCount) == API_OK && outputCount > 0) {
                m_foundDevices.push_back({devId, streamCount});
                m_deviceList->Append(wxString::Format("Device ID: %u (Found %u streams)", devId, streamCount));
            }
            ApiClose(tempHandle);
        }
    }

    // Tarama bitti, API'yi serbest bırakıyoruz.
    ApiExit();

    if (m_foundDevices.empty()) {
        m_deviceList->Append("No AIM devices found.");
    }
}

void DeviceSelectionPanel::populateStreamChoices(wxChoice* choice, AiUInt32 streamCount) {
    choice->Clear();
    for (AiUInt32 i = 1; i <= streamCount; ++i) {
        choice->Append(wxString::Format("Stream %u", i));
    }
    if (streamCount > 0) {
        choice->SetSelection(0);
    }
}

void DeviceSelectionPanel::onInitialize(wxCommandEvent& event) {
    int deviceSel = m_deviceList->GetSelection();
    int bcStreamSel = m_bcStreamChoice->GetSelection();
    int bmStreamSel = m_bmStreamChoice->GetSelection();

    if (deviceSel == wxNOT_FOUND || bcStreamSel == wxNOT_FOUND || bmStreamSel == wxNOT_FOUND) {
        return;
    }
    
    if (bcStreamSel == bmStreamSel) {
        wxMessageBox("Bus Controller and Bus Monitor must use different streams.", "Selection Error", wxOK | wxICON_ERROR);
        return;
    }

    AiUInt32 deviceId = m_foundDevices[deviceSel].deviceId;
    AiUInt32 bcStreamId = bcStreamSel + 1;
    AiUInt32 bmStreamId = bmStreamSel + 1;

    auto* mainFrame = dynamic_cast<MainFrame*>(wxGetTopLevelParent(this));
    if (mainFrame) {
        mainFrame->InitializePanels(deviceId, bcStreamId, bmStreamId);
    }
}