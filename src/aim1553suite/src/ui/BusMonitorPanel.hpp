#pragma once

#include <wx/panel.h>       // Temel sınıfımız artık wxPanel
#include <wx/treectrl.h>    // RT/SA/MC ağaç yapısı için
#include <wx/textctrl.h>    // Mesaj listesi ve Device ID girişi için
#include <wx/button.h>      // Kontrol butonları için
#include <wx/checkbox.h>    // Log to File seçeneği için
#include <wx/combobox.h>    // Stream seçimi için
#include <map>              // Ağaç elemanlarını mode code'lara eşlemek için

#include "Api1553.h"        // AiUInt32 gibi AIM tipleri için

// Tam başlık dosyalarını dahil etmek yerine forward declaration yapıyoruz.
class MainFrame;
class wxTreeEvent;

// Eski BusMonitorFrame sınıfı, artık bir wxPanel olarak yeniden yapılandırıldı.
class BusMonitorPanel : public wxPanel {
public:
    // Kurucu fonksiyon artık bir ebeveyn pencere (notebook) alıyor.
    BusMonitorPanel(wxWindow* parent);

    // Yıkıcı fonksiyon, panel yok edildiğinde izlemenin durdurulduğundan
    // emin olmak için önemlidir.
    ~BusMonitorPanel();

private:
    // Bu paneldeki widget'lara ait olay yöneticisi fonksiyonları.
    void onStartStopClicked(wxCommandEvent &event);
    void onClearFilterClicked(wxCommandEvent &event);
    void onClearClicked(wxCommandEvent &event);
    void onTreeItemClicked(wxTreeEvent &event);
    void onLogToFileToggled(wxCommandEvent &event);
    void onResetStreamClicked(wxCommandEvent &event);

    // Arayüzü güncellemek için kullanılan yardımcı metotlar.
    // Bunlar genellikle arka plan iş parçacığından (thread) güvenli bir şekilde çağrılır.
    void appendMessagesToUi(const wxString& messages);
    void updateTreeItemVisualState(char bus, int rt, int sa, bool isActive);
    void resetTreeVisualState();

    // Ana çerçevenin durum çubuğunu güncellemek için bir yardımcı metot.
    void setStatusText(const wxString& text);

    // Ana uygulama çerçevesine (MainFrame) bir işaretçi.
    MainFrame* m_mainFrame;

    // Arayüz bileşenlerini (widgets) tutan üye değişkenler.
    int m_uiRecentMessageCount;
    wxTextCtrl *m_deviceIdTextInput;
    wxTreeCtrl *m_milStd1553Tree;
    wxTextCtrl *m_messageList;
    wxButton *m_startStopButton;
    wxButton *m_filterButton;
    wxCheckBox *m_logToFileCheckBox;
    wxComboBox *m_streamChoiceComboBox;
    wxButton *m_resetStreamButton;

    // Panel'in iç mantığı için gereken veri üyeleri.
    std::map<wxTreeItemId, int> m_treeItemToMcMap;
    AiUInt32 m_totalStreams;
};