#pragma once

#include <mutex>
#include <wx/treectrl.h>
#include <wx/wx.h>
#include "common.hpp"
#include "logger.hpp"
#include <map>
#include "Api1553.h" // YENİ: AiUInt32 gibi AIM tiplerinin tanınması için eklendi

enum {
  ID_ADD_BTN = 1,
  ID_ADD_MENU,
  ID_FILTER_BTN,
  ID_CLEAR_BTN,
  ID_FILTER_MENU,
  ID_CLEAR_MENU,
  ID_DEVICE_ID_TXT,
  ID_RT_SA_TREE,
  ID_LOG_TO_FILE_CHECKBOX,
  ID_STREAM_CHOICE_COMBOBOX,
  ID_RESET_STREAM_BTN
};

class BusMonitorFrame : public wxFrame {
public:
  BusMonitorFrame();
  ~BusMonitorFrame();

private:
  void onStartStopClicked(wxCommandEvent &event);
  void onClearFilterClicked(wxCommandEvent &event);
  void onClearClicked(wxCommandEvent &event);
  void onTreeItemClicked(wxTreeEvent &event);
  void onExit(wxCommandEvent &event);
  void onLogToFileToggled(wxCommandEvent &event); 
  void onCloseFrame(wxCloseEvent& event);
  void onResetStreamClicked(wxCommandEvent &event);

  void appendMessagesToUi(const wxString& messages);
  void updateTreeItemVisualState(char bus, int rt, int sa, bool isActive);
  void resetTreeVisualState();

  int m_uiRecentMessageCount;
  wxTextCtrl *m_deviceIdTextInput;
  wxTreeCtrl *m_milStd1553Tree;
  wxTextCtrl *m_messageList;
  wxButton *m_startStopButton;
  wxButton *m_filterButton;
  wxCheckBox *m_logToFileCheckBox;
  std::map<wxTreeItemId, int> m_treeItemToMcMap; 

  // DÜZELTİLDİ: AiUInt32 artık tanınıyor
  AiUInt32 m_totalStreams;
  wxComboBox *m_streamChoiceComboBox;
  wxButton *m_resetStreamButton;

  wxDECLARE_EVENT_TABLE();
};