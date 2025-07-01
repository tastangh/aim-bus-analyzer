// fileName: ui/mainWindow.hpp
#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include "ModeSelectionDialog.hpp"

// Paneller için ileriye dönük bildirim (Forward declarations)
class BCPanel;
class RTPanel;
class BMPanel;
class ScenarioPanel; // DÜZELTME: Bu satır eklendi.

class MainWindow : public wxFrame {
public:
    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    void SetStatusText(const wxString& text);

private:
    void CreateUIPanels(const AppConfig& config);

    wxNotebook* m_notebook;

    // Her mod için oluşturulacak panel işaretçileri
    BCPanel* m_bcPanel;
    RTPanel* m_rtPanel;
    BMPanel* m_bmPanel;
    ScenarioPanel* m_scenarioPanel; // DÜZELTME: Bu satır eklendi.
};