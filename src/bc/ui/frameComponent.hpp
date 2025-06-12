#pragma once

#include "../../common.hpp"
#include "Api1553.h"
#include <wx/tglbtn.h>
#include <wx/wx.h>

class BusControllerFrame;

class FrameComponent : public wxPanel {
public:
  explicit FrameComponent(wxWindow *parent, const FrameConfig &config);
  void updateValues(const FrameConfig &config);
  void sendFrame();
  bool isActive() const;
  const FrameConfig &getFrameConfig() const { return m_config; }
  void updateData(const std::array<AiUInt16, BC_MAX_DATA_WORDS> &newData);

private:
  void onSend(wxCommandEvent &event);
  void onRemove(wxCommandEvent &event);
  void onEdit(wxCommandEvent &event);
  void onActivateToggle(wxCommandEvent &event);

  BusControllerFrame *m_mainWindow;
  wxStaticText *m_allText{};
  wxToggleButton *m_activateToggle{};
  FrameConfig m_config;
};