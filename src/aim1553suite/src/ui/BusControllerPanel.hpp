#pragma once

#include "common.hpp"
#include <wx/panel.h>
#include <wx/tglbtn.h>
#include <wx/scrolwin.h>
#include <thread>
#include <atomic>
#include <vector>

class FrameComponent;
class wxBoxSizer;
class MainFrame;

class BusControllerPanel : public wxPanel {
public:
    BusControllerPanel(wxWindow* parent);
    ~BusControllerPanel();

    // MainFrame tarafından çağrılacak yeni fonksiyon
    void InitializeHardware(unsigned int deviceId, unsigned int streamId);

    // Dışarıdan erişim için gerekli fonksiyonlar
    void addFrameToList(FrameConfig config);
    void removeFrame(FrameComponent* frame);
    void updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig);
    void updateListLayout();
    void setStatusText(const wxString &status);
    unsigned int getDeviceId() const { return m_deviceId; }

    
private:
    void onAddFrameClicked(wxCommandEvent &event);
    void onClearFramesClicked(wxCommandEvent &event);
    void onRepeatToggle(wxCommandEvent &event);
    void onSendActiveFramesToggle(wxCommandEvent &event);

    void sendActiveFramesLoop();
    void startSendingThread();
    void stopSendingThread();
    
    MainFrame* m_mainFrame;

    // Donanım bilgileri artık burada saklanacak
    unsigned int m_deviceId;
    unsigned int m_streamId;

    // Arayüz elemanları
    wxToggleButton *m_repeatToggle;
    wxToggleButton *m_sendActiveFramesToggle;
    wxScrolledWindow *m_scrolledWindow;
    wxBoxSizer *m_scrolledSizer;

    // Arka plan işlemleri
    std::thread m_sendThread;
    std::atomic<bool> m_isSending{false};
    std::atomic<bool> m_isRepeatOn{false}; 
    std::vector<FrameComponent*> m_frameComponents;
};