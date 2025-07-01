// fileName: ui/BCPanel.hpp
#pragma once

#include "../bc_common.hpp"
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/scrolwin.h>
#include <vector>
#include <thread>
#include <atomic>

class FrameComponent; // Forward declaration
class MainWindow;     // Forward declaration

class BCPanel : public wxPanel {
public:
    BCPanel(wxWindow* parent);
    ~BCPanel();

    // FrameComponent ve CreateFrameWindow tarafından çağrılacak public fonksiyonlar
    void addFrameToList(const FrameConfig& config);
    void updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig);
    void removeFrame(FrameComponent* frame);
    void updateListLayout();
    int getDeviceId();
    void setStatusText(const wxString& text);

private:
    // Olay Yöneticileri (Event Handlers)
    void onAddFrameClicked(wxCommandEvent &event);
    void onClearFramesClicked(wxCommandEvent &event);
    void onRepeatToggle(wxCommandEvent &event);
    void onSendActiveFramesToggle(wxCommandEvent &event);
    
    // Arka plan işlemleri için (şimdilik boş)
    void startSendingThread();
    void stopSendingThread();
    void sendActiveFramesLoop();

    // Arayüz Elemanları
    wxTextCtrl *m_deviceIdTextInput;
    wxToggleButton *m_repeatToggle;
    wxToggleButton *m_sendActiveFramesToggle;
    wxScrolledWindow *m_scrolledWindow;
    wxBoxSizer *m_scrolledSizer;

    // Durum değişkenleri ve veri
    std::vector<FrameComponent*> m_frameComponents;
    std::thread m_sendThread;
    std::atomic<bool> m_isSending{false};

    MainWindow* m_mainFrame; // Ana pencereye erişim için
};