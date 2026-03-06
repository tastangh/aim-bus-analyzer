#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>

class BusControllerPanel;
class BusMonitorPanel;
class DeviceSelectionPanel;

class MainFrame : public wxFrame {
public:
    MainFrame();
    void InitializePanels(unsigned int deviceId, unsigned int bcStreamId, unsigned int bmStreamId);

private:
    void onExit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);

    wxNotebook* m_notebook;
    BusControllerPanel* m_bcPanel;
    BusMonitorPanel* m_bmPanel;
    DeviceSelectionPanel* m_devicePanel;
};