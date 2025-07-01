// fileName: ui/BMPanel.hpp
#pragma once

#include "../bm_common.hpp"
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/checkbox.h>
#include <map>

class MainWindow;

class BMPanel : public wxPanel {
public:
  BMPanel(wxWindow *parent);
  ~BMPanel();

private:
  void onStartStopClicked(wxCommandEvent &event);
  void onClearFilterClicked(wxCommandEvent &event);
  void onClearClicked(wxCommandEvent &event);
  void onTreeItemClicked(wxTreeEvent &event);
  void onLogToFileToggled(wxCommandEvent &event); 

  void appendMessagesToUi(const wxString& messages);
  void updateTreeItemVisualState(char bus, int rt, int sa, bool isActive);
  void resetTreeVisualState();

  MainWindow* m_mainFrame;
  int m_uiRecentMessageCount;

  wxTextCtrl *m_deviceIdTextInput;
  wxTreeCtrl *m_milStd1553Tree;
  wxTextCtrl *m_messageList;
  wxButton *m_startStopButton;
  wxButton *m_filterButton;
  wxCheckBox *m_logToFileCheckBox;
  
  std::map<wxTreeItemId, int> m_treeItemToMcMap; 
};