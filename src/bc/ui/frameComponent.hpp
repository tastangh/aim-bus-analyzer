// fileName: frameComponent.hpp
#pragma once
#include "common.hpp"
#include "AiOs.h"
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <array>

class BusControllerFrame;

class FrameComponent : public wxPanel {
public:
  explicit FrameComponent(wxWindow *parent, const FrameConfig &config);
  void updateValues(const FrameConfig &config);
  void sendFrame();
  bool isActive() const;
  const FrameConfig &getFrameConfig() const { return m_config; }
  void updateDataUI(const std::array<AiUInt16, BC_MAX_DATA_WORDS> &newData);

  void setAimIds(AiUInt16 xferId, AiUInt16 hdrId, AiUInt16 bufId);
  AiUInt16 getAimTransferId() const { return m_aimTransferId; }
  AiUInt16 getAimHeaderId() const { return m_aimHeaderId; }
  AiUInt16 getAimBufferId() const { return m_aimBufferId; }

private:
  void onSend(wxCommandEvent &event);
  void onRemove(wxCommandEvent &event);
  void onEdit(wxCommandEvent &event);
  void onActivateToggle(wxCommandEvent &event);

  BusControllerFrame *m_mainWindow;
  wxStaticText *m_summaryText{};
  std::array<wxStaticText*, BC_MAX_DATA_WORDS> m_dataLabels;
  wxToggleButton *m_activateToggle{};
  FrameConfig m_config;

  AiUInt16 m_aimTransferId = 0;
  AiUInt16 m_aimHeaderId = 0;
  AiUInt16 m_aimBufferId = 0;
};