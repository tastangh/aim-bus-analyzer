#pragma once

#include <mutex>
#include <wx/treectrl.h>
#include <wx/wx.h>

// Enum ID'ler burada veya .cpp içinde olabilir. Başlıkta olması daha yaygın.
enum {
  ID_ADD_BTN = 1,
  ID_ADD_MENU,
  ID_FILTER_BTN,
  ID_CLEAR_BTN,
  ID_FILTER_MENU,
  ID_CLEAR_MENU,
  ID_DEVICE_ID_TXT,
  ID_RT_SA_TREE
};

// mainWindow.cpp içindeki TOP_BAR_COMP_HEIGHT yerine
const int TOP_BAR_COMP_HEIGHT = 28; // Veya uygun bir değer


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
  void onCloseFrame(wxCloseEvent& event);

  void appendMessagesToUi(const wxString& messages);
  void updateTreeItemVisualState(char bus, int rt, int sa, bool isActive);

  int m_uiRecentMessageCount;
  // std::mutex m_uiMutex; // wxTheApp->CallAfter UI thread senkronizasyonunu sağlar
  wxTextCtrl *m_deviceIdTextInput;
  wxTreeCtrl *m_milStd1553Tree;
  wxTextCtrl *m_messageList;
  wxButton *m_startStopButton;
  wxButton *m_filterButton;

  wxDECLARE_EVENT_TABLE();
};