// fileName: ui/ScenarioPanel.hpp
#pragma once

#include <wx/wx.h>

class MainWindow;

class ScenarioPanel : public wxPanel {
public:
    ScenarioPanel(wxWindow *parent);

private:
    void OnStartScenario(wxCommandEvent& event);

    MainWindow* m_mainFrame;
    wxTextCtrl* m_deviceIdTextInput; 
    wxButton* m_startButton;
    wxTextCtrl* m_logText;
};