#pragma once

#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <vector>
#include "Api1553.h"

// Donanım bilgilerini tutacak basit bir yapı
struct DeviceInfo {
    AiUInt32 deviceId;
    AiUInt32 streamCount;
};

class DeviceSelectionPanel : public wxPanel {
public:
    DeviceSelectionPanel(wxWindow* parent);

private:
    void onScanHardware(wxCommandEvent& event);
    void onInitialize(wxCommandEvent& event);
    void populateStreamChoices(wxChoice* choice, AiUInt32 streamCount);

    std::vector<DeviceInfo> m_foundDevices;

    wxListBox* m_deviceList;
    wxChoice* m_bcStreamChoice;
    wxChoice* m_bmStreamChoice;
    wxButton* m_scanButton;
    wxButton* m_initializeButton;
};