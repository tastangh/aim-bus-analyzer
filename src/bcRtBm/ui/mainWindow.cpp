// fileName: ui/mainWindow.cpp
#include "mainWindow.hpp"
#include "ModeSelectionDialog.hpp"

// Panel sınıflarını dahil et (şimdilik sadece basit tanımlar)
class BCPanel : public wxPanel { public: BCPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) { new wxStaticText(this, wxID_ANY, "Bus Controller Panel", wxPoint(20, 20)); } };
class RTPanel : public wxPanel { public: RTPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) { new wxStaticText(this, wxID_ANY, "Remote Terminal Panel", wxPoint(20, 20)); } };
class BMPanel : public wxPanel { public: BMPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) { new wxStaticText(this, wxID_ANY, "Bus Monitor Panel", wxPoint(20, 20)); } };


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    
    // Mod seçim diyalogunu göster
    ModeSelectionDialog dialog(this, wxID_ANY, "Select Operation Mode");
    
    if (dialog.ShowModal() == wxID_OK) {
        // Kullanıcı OK'a bastıysa, seçilen konfigürasyonu al ve arayüzü oluştur
        AppConfig config = dialog.GetConfiguration();
        CreateUIPanels(config);
    } else {
        // Kullanıcı Cancel'a bastıysa veya pencereyi kapattıysa uygulamadan çık
        Close();
    }
}

void MainWindow::CreateUIPanels(const AppConfig& config) {
    // Ana pencereye bir Notebook (sekmeli yapı) ekle
    m_notebook = new wxNotebook(this, wxID_ANY);

    if (config.bc_selected) {
        m_bcPanel = new BCPanel(m_notebook);
        m_notebook->AddPage(m_bcPanel, "Bus Controller", true);
    }

    if (config.rt_selected) {
        m_rtPanel = new RTPanel(m_notebook);
        m_notebook->AddPage(m_rtPanel, "Remote Terminal", !config.bc_selected);
    }

    if (config.bm_selected) {
        m_bmPanel = new BMPanel(m_notebook);
        m_notebook->AddPage(m_bmPanel, "Bus Monitor", !(config.bc_selected || config.rt_selected));
    }
    
    // Eğer hiçbir şey seçilmediyse (çoklu modda mümkün) bir uyarı göster
    if (m_notebook->GetPageCount() == 0) {
        wxPanel* emptyPanel = new wxPanel(m_notebook);
        new wxStaticText(emptyPanel, wxID_ANY, "No operation mode was selected.", wxPoint(20,20));
        m_notebook->AddPage(emptyPanel, "Info");
    }

    CreateStatusBar();
    SetStatusText("Ready.");
}