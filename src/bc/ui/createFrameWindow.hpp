#pragma once

#include "../../common.hpp"
#include <wx/wx.h>
#include <vector>

class FrameComponent;
class BusControllerFrame;

class FrameCreationFrame : public wxFrame {
public:
  explicit FrameCreationFrame(BusControllerFrame *parent);
  explicit FrameCreationFrame(BusControllerFrame *parent, FrameComponent *frameToEdit);

private:
  void createAndLayoutControls();
  void onSave(wxCommandEvent &event);
  void onWcChanged(wxCommandEvent &event);
  void onModeChanged(wxCommandEvent &event);
  void onRandomize(wxCommandEvent &event);
  void onClose(wxCommandEvent &event);
  void populateFieldsFromConfig(const FrameConfig &config);
  FrameConfig buildConfigFromFields();
  void updateControlStates();

  BusControllerFrame *m_parentFrame;
  FrameComponent *m_editingFrame = nullptr;
  wxBoxSizer *m_mainSizer{};
  wxBoxSizer *m_cmdWord2Sizer{};
  wxButton *m_saveButton{};
  wxComboBox *m_busCombo{};
  wxComboBox *m_rtCombo{};
  wxComboBox *m_rt2Combo{};
  wxComboBox *m_saCombo{};
  wxComboBox *m_sa2Combo{};
  wxComboBox *m_wcCombo{};
  wxComboBox *m_modeCombo{};
  wxTextCtrl *m_labelTextCtrl{};
  std::vector<wxTextCtrl *> m_dataTextCtrls;
};