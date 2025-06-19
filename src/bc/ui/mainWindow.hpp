// fileName: mainWindow.hpp
#pragma once

#include "common.hpp"
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/scrolwin.h>
#include <thread>
#include <atomic>
#include <vector>
#include <future>
#include <memory>

class FrameComponent;

class BusControllerFrame : public wxFrame {
public:
  BusControllerFrame();
  ~BusControllerFrame();

  void addFrameToList(FrameConfig config); // Değişiklik yapılabilmesi için by-value
  void removeFrame(FrameComponent* frame);
  void updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig);
  
  void setStatusText(const wxString &status);
  int getDeviceId();
  void updateListLayout();

private:
  void onAddFrameClicked(wxCommandEvent &event);
  void onClearFramesClicked(wxCommandEvent &event);
  void onRepeatToggle(wxCommandEvent &event);
  void onSendActiveFramesToggle(wxCommandEvent &event);
  void onExit(wxCommandEvent &event);
  void onCloseFrame(wxCloseEvent &event);

  void sendActiveFramesLoop();
  void startSendingThread();
  void stopSendingThread();

  wxTextCtrl *m_deviceIdTextInput;
  wxToggleButton *m_repeatToggle;
  wxToggleButton *m_sendActiveFramesToggle;
  wxScrolledWindow *m_scrolledWindow;
  wxBoxSizer *m_scrolledSizer;

  std::thread m_sendThread;
  std::atomic<bool> m_isSending{false};
  std::vector<FrameComponent*> m_frameComponents;
};