#pragma once

#include "common.hpp"
#include <wx/panel.h>
#include <wx/tglbtn.h>
#include <wx/scrolwin.h>
#include <wx/textctrl.h> // HATA ÇÖZÜMÜ: Eksik başlık dosyası eklendi.

#include <thread>
#include <atomic>
#include <vector>
#include <future>
#include <memory>

class FrameComponent;
class wxBoxSizer;
class MainFrame;

class BusControllerPanel : public wxPanel {
public:
    BusControllerPanel(wxWindow* parent);
    ~BusControllerPanel();

    void addFrameToList(FrameConfig config);
    void removeFrame(FrameComponent* frame);
    void updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig);
    void updateListLayout();
    void setStatusText(const wxString &status);

    // HATA ÇÖZÜMÜ: Bu fonksiyon private alanından public alanına taşındı.
    int getDeviceId(); 

private:
    void onAddFrameClicked(wxCommandEvent &event);
    void onClearFramesClicked(wxCommandEvent &event);
    void onRepeatToggle(wxCommandEvent &event);
    void onSendActiveFramesToggle(wxCommandEvent &event);

    void sendActiveFramesLoop();
    void startSendingThread();
    void stopSendingThread();
    
    MainFrame* m_mainFrame;

    wxTextCtrl *m_deviceIdTextInput;
    wxToggleButton *m_repeatToggle;
    wxToggleButton *m_sendActiveFramesToggle;
    wxScrolledWindow *m_scrolledWindow;
    wxBoxSizer *m_scrolledSizer;

    std::thread m_sendThread;
    std::atomic<bool> m_isSending{false};
    // HATA ÇÖZÜMÜ: Thread-safe olmayan GUI erişimini düzeltmek için eklendi.
    std::atomic<bool> m_isRepeatOn{false}; 
    std::vector<FrameComponent*> m_frameComponents;
};