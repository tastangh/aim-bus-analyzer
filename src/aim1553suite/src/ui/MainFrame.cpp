#include "MainFrame.hpp"
#include "BusControllerPanel.hpp" // Bus Controller arayüzünü içeren Panel
#include "BusMonitorPanel.hpp"    // Bus Monitor arayüzünü içeren Panel
#include "bc.hpp"            // BusController singleton'ını kapatma işlemleri için
#include "bm.hpp"            // BusMonitor singleton'ını kapatma işlemleri için
#include <wx/notebook.h>         // Sekmeli arayüz için

// Olayları (Events) bağlamak için ID'ler tanımlıyoruz.
enum {
    ID_Exit = 1
};

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "AIM 1553 Suite", wxDefaultPosition, wxSize(1200, 800)) {
    
    // 1. Menü Çubuğunu Oluşturma
    //----------------------------------------------------
    auto *menuFile = new wxMenu;
    menuFile->Append(ID_Exit, "E&xit\tAlt-X", "Uygulamayı kapat");

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");

    SetMenuBar(menuBar);

    // 2. Durum Çubuğunu (Status Bar) Oluşturma
    //----------------------------------------------------
    CreateStatusBar();
    SetStatusText("Ready");

    // 3. Sekmeli Arayüzü (Notebook) Oluşturma
    //----------------------------------------------------
    m_notebook = new wxNotebook(this, wxID_ANY);

    // 4. Panelleri Oluşturma ve Notebook'a Ekleme
    //----------------------------------------------------
    // Bus Controller ve Bus Monitor arayüzlerini ayrı paneller olarak oluşturuyoruz.
    // Bu panellerin "ebeveyni" (parent) artık notebook'un kendisidir.
    auto* controllerPanel = new BusControllerPanel(m_notebook);
    auto* monitorPanel = new BusMonitorPanel(m_notebook);

    // Oluşturulan panelleri sekmeler halinde notebook'a ekliyoruz.
    // "true" parametresi, bu sekmenin başlangıçta seçili olmasını sağlar.
    m_notebook->AddPage(controllerPanel, "Bus Controller", true);
    m_notebook->AddPage(monitorPanel, "Bus Monitor", false);

    // 5. Pencere Düzenini (Layout) Ayarlama
    //----------------------------------------------------
    // Notebook'un pencere boyutuna göre otomatik genişlemesini sağlamak için
    // bir sizer kullanıyoruz.
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    // '1' parametresi notebook'un dikeyde mevcut tüm alanı kaplamasını sağlar (proportion).
    // wxEXPAND, hem yatayda hem dikeyde genişlemesini sağlar.
    mainSizer->Add(m_notebook, 1, wxEXPAND); 
    SetSizer(mainSizer);

    // 6. Olayları (Events) Fonksiyonlara Bağlama
    //----------------------------------------------------
    // Menüdeki "Exit" seçeneğine tıklandığında onExit fonksiyonunu çağır.
    Bind(wxEVT_MENU, &MainFrame::onExit, this, ID_Exit);

    // Pencerenin kapatma düğmesine (X) basıldığında veya program kapatıldığında
    // onClose fonksiyonunu çağır. Bu, kaynakları güvenli bir şekilde temizlemek için kritiktir.
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::onClose, this);

    // 7. Pencereyi Ortala ve Göster
    //----------------------------------------------------
    Centre();
    Layout(); // Sizer'ın düzgün çalışması için layout'u güncelliyoruz.
}

// Menüdeki Exit butonuna basıldığında tetiklenir.
void MainFrame::onExit(wxCommandEvent& event) {
    // Pencereyi kapatır. Bu, wxEVT_CLOSE_WINDOW olayını tetikler.
    Close(true); 
}

// Pencere kapatıldığında tetiklenir.
void MainFrame::onClose(wxCloseEvent& event) {
    // Bu fonksiyon, uygulama kapanırken kaynak sızıntısı veya çökme olmaması için hayati önem taşır.
    
    // Eğer Bus Monitor arkaplan iş parçacığında (thread) çalışıyorsa, onu güvenli bir şekilde durdur.
    if (BM::getInstance().isMonitoring()) {
        BM::getInstance().stop();
    }

    // Bus Controller'ın kullandığı AIM donanım kaynaklarını serbest bırak.
    BusController::getInstance().shutdown();

    // Tüm temizlik işlemleri bittikten sonra pencereyi yok et.
    Destroy();
}