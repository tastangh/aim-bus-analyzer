// fileName: ui/mainWindow.hpp
#pragma once

#include "app.hpp"
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/tglbtn.h>
#include <vector>

class FrameComponent;

class MainWindow : public wxFrame {
public:
  MainWindow();
  ~MainWindow();

  void addFrame(FrameConfig config);
  void removeFrame(FrameComponent* component);
  void updateFrame(FrameComponent* component, const FrameConfig& newConfig);
  
  void updateListLayout();
  int getDeviceId();
  int getRtAddress(); // Bu fonksiyon arkaplanda RT adresini almak için kalacak

private:
  void onAdd(wxCommandEvent &event);
  void onClear(wxCommandEvent &event);
  void onStartStopToggle(wxCommandEvent &event); // Olay yöneticisi RT'yi başlatıp durduracak
  void onExit(wxCommandEvent &event);
  void onCloseFrame(wxCloseEvent &event);

  // Arayüz elemanları BC versiyonuna benzetildi
  wxTextCtrl *m_deviceIdText;
  wxTextCtrl *m_rtAddressText; // Bu kontrol gizli olacak ama RT adresi için kullanılacak
  wxToggleButton *m_startStopToggle;
  wxToggleButton *m_repeatToggle;
  
  wxScrolledWindow *m_scrolledWindow;
  wxBoxSizer *m_scrolledSizer;

  std::vector<FrameComponent*> m_frameComponents;
};