#pragma once

#include "common.hpp"
#include <atomic>
#include <thread>
#include <vector>
#include <wx/tglbtn.h>
#include <wx/wx.h>

class FrameComponent;

class BusControllerFrame : public wxFrame {
public:
  BusControllerFrame();
  ~BusControllerFrame();

  void addFrameToList(const FrameConfig &config);
  void removeFrame(FrameComponent* frame);
  void updateFrame(FrameComponent* oldFrame, const FrameConfig& newConfig);
  
  void setStatusText(const wxString &status);
  int getDeviceId();
  void updateListLayout();

private:
  // --- Event Handlers ---
  void onAddFrameClicked(wxCommandEvent &event);
  void onClearFramesClicked(wxCommandEvent &event);
  void onRepeatToggle(wxCommandEvent &event);
  void onSendActiveFramesToggle(wxCommandEvent &event);
  void onLoadFrames(wxCommandEvent &event);
  void onSaveFrames(wxCommandEvent &event);
  void onExit(wxCommandEvent &event);
  void onCloseFrame(wxCloseEvent &event);

  // --- Thread & Send Logic ---
  void sendActiveFramesLoop();
  void startSendingThread();
  void stopSendingThread();

  // --- UI Components ---
  wxTextCtrl *m_deviceIdTextInput;
  wxToggleButton *m_repeatToggle;
  wxToggleButton *m_sendActiveFramesToggle;
  wxScrolledWindow *m_scrolledWindow;
  wxBoxSizer *m_scrolledSizer;

  // --- State Management ---
  std::thread m_sendThread;
  std::atomic<bool> m_isSending{false};
};