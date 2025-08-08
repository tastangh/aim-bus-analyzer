#pragma once

#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <map>
#include "Api1553.h"

class MainFrame;
class wxTreeEvent;

class BusMonitorPanel : public wxPanel {
public:
    BusMonitorPanel(wxWindow* parent);
    ~BusMonitorPanel();

    // MainFrame tarafından çağrılacak yeni fonksiyon
    void InitializeHardware(unsigned int deviceId, unsigned int streamId);

private:
    void onStartStopClicked(wxCommandEvent &event);
    void onClearFilterClicked(wxCommandEvent &event);
    void onClearClicked(wxCommandEvent &event);
    void onTreeItemClicked(wxTreeEvent &event);
    void onLogToFileToggled(wxCommandEvent &event);
    void onResetStreamClicked(wxCommandEvent &event);

    void appendMessagesToUi(const wxString& messages);
    void updateTreeItemVisualState(char bus, int rt, int sa, bool isActive);
    void resetTreeVisualState();
    void setStatusText(const wxString& text);

    MainFrame* m_mainFrame;

    // Donanım bilgileri artık burada saklanacak
    unsigned int m_deviceId;
    unsigned int m_streamId;

    // Arayüz ve veri üyeleri
    int m_uiRecentMessageCount;
    wxTreeCtrl *m_milStd1553Tree;
    wxTextCtrl *m_messageList;
    wxButton *m_startStopButton;
    wxButton *m_filterButton;
    wxCheckBox *m_logToFileCheckBox;
    wxButton *m_resetStreamButton;
    std::map<wxTreeItemId, int> m_treeItemToMcMap;
};