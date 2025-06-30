// fileName: ui/frameComponent.hpp
#pragma once
#include "app.hpp"
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <array>

class MainWindow;

class FrameComponent : public wxPanel {
public:
  explicit FrameComponent(wxWindow *parent, const FrameConfig &config);
  
  void updateValues(const FrameConfig &config);
  const FrameConfig &getConfig() const { return m_config; }

  void setAimIds(AiUInt16 hdrId, AiUInt16 bufId);
  AiUInt16 getAimHeaderId() const { return m_aimHeaderId; }
  AiUInt16 getAimBufferId() const { return m_aimBufferId; }

private:
  void onRemove(wxCommandEvent &event);
  void onEdit(wxCommandEvent &event);
  void onEnableToggle(wxCommandEvent &event);
  void onSendOnce(wxCommandEvent &event); // YENİ

  MainWindow *m_mainFrame;
  wxStaticText *m_summaryText{};
  std::array<wxStaticText*, 32> m_dataLabels;
  wxToggleButton *m_enableToggle{};
  FrameConfig m_config;

  AiUInt16 m_aimHeaderId = 0;
  AiUInt16 m_aimBufferId = 0;
};