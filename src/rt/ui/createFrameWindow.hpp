// fileName: ui/createFrameWindow.hpp
#pragma once

#include "app.hpp"
#include <wx/wx.h>
#include <wx/combobox.h>
#include <vector>

class MainWindow;
class FrameComponent;

class CreateFrameWindow : public wxFrame {
public:
  explicit CreateFrameWindow(MainWindow *parent);
  explicit CreateFrameWindow(MainWindow *parent, FrameComponent *frameToEdit);

private:
  void createAndLayoutControls();
  void onSave(wxCommandEvent &event);
  void onRandomize(wxCommandEvent &event);
  void onClose(wxCommandEvent &event);
  
  void populateFields(const FrameConfig &config);
  FrameConfig buildConfigFromFields();
  
  MainWindow *m_mainFrame;
  FrameComponent *m_editingComponent = nullptr;
  
  wxButton *m_saveButton{};
  wxComboBox *m_busCombo{};
  wxComboBox *m_rtCombo{};
  wxComboBox *m_saCombo{};
  wxComboBox *m_wcCombo{};
  wxTextCtrl *m_labelTextCtrl{};
  std::vector<wxTextCtrl *> m_dataTextCtrls;
};