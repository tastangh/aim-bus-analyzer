// fileName: ui/mainWindow.hpp
#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include "ModeSelectionDialog.hpp"

// Forward declarations
class BCPanel;
class RTPanel;
class BMPanel;

class MainWindow : public wxFrame {
public:
    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void CreateUIPanels(const AppConfig& config);

    wxNotebook* m_notebook;
    BCPanel* m_bcPanel;
    RTPanel* m_rtPanel;
    BMPanel* m_bmPanel;
};