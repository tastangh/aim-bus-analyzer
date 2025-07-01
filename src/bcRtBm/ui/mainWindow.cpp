// fileName: ui/mainWindow.cpp
#include "mainWindow.hpp"
#include "ModeSelectionDialog.hpp"
#include "BCPanel.hpp"
#include "BMPanel.hpp"
#include "ScenarioPanel.hpp"

// RTPanel için geçici iskelet sınıf
class RTPanel : public wxPanel { 
public: 
    RTPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) { 
        SetBackgroundColour(*wxLIGHT_GREY);
        wxStaticText* text = new wxStaticText(this, wxID_ANY, "Remote Terminal (RT) Panel - UI TO BE IMPLEMENTED", wxPoint(20, 20));
        wxFont font = text->GetFont(); font.SetPointSize(14); font.SetWeight(wxFONTWEIGHT_BOLD); text->SetFont(font);
    } 
};

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    
    CreateStatusBar();
    SetStatusText("Initializing...");

    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_ANY, "Select Mode...", "Select operation modes again.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    ModeSelectionDialog dialog(this, wxID_ANY, "Select Operation Mode");
    if (dialog.ShowModal() == wxID_OK) {
        AppConfig config = dialog.GetConfiguration();
        CreateUIPanels(config);
    } else {
        Close();
    }
}

void MainWindow::CreateUIPanels(const AppConfig& config) {
    m_notebook = new wxNotebook(this, wxID_ANY);

    // DÜZELTME: if/else yapısı ile mantık hatası giderildi.
    if (config.mainMode == OperationMode::SCENARIO_BC_RT_BM) {
        // Sadece Senaryo paneli oluşturulur.
        m_scenarioPanel = new ScenarioPanel(m_notebook);
        m_notebook->AddPage(m_scenarioPanel, "Concurrent Scenario", true);
        SetStatusText("Scenario mode ready.");
    } else {
        // Tekli veya Özel Çoklu mod için paneller oluşturulur.
        bool isFirstPanel = true;
        if (config.bc_selected) {
            m_bcPanel = new BCPanel(m_notebook);
            m_notebook->AddPage(m_bcPanel, "Bus Controller", isFirstPanel);
            isFirstPanel = false;
        }
        if (config.rt_selected) {
            m_rtPanel = new RTPanel(m_notebook);
            m_notebook->AddPage(m_rtPanel, "Remote Terminal", isFirstPanel);
            isFirstPanel = false;
        }
        if (config.bm_selected) {
            m_bmPanel = new BMPanel(m_notebook);
            m_notebook->AddPage(m_bmPanel, "Bus Monitor", isFirstPanel);
            isFirstPanel = false;
        }
        
        if (m_notebook->GetPageCount() == 0) {
            wxPanel* emptyPanel = new wxPanel(m_notebook);
            new wxStaticText(emptyPanel, wxID_ANY, "No operation mode was selected.", wxPoint(20,20));
            m_notebook->AddPage(emptyPanel, "Info");
            SetStatusText("No mode selected.");
        } else {
            SetStatusText("Ready.");
        }
    }
}

void MainWindow::SetStatusText(const wxString& text) {
    wxFrame::SetStatusText(text);
}