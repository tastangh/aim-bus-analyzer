// fileName: ui/frameComponent.hpp
#pragma once

#include "../bc_common.hpp"
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <array>

class BCPanel;

class FrameComponent : public wxPanel {
public:
    explicit FrameComponent(wxWindow *parent, BCPanel* mainPanel, const FrameConfig &config);
    void updateValues(const FrameConfig &config);
    const FrameConfig &getFrameConfig() const { return m_config; }
    bool isActive() const;

private:
    void onSend(wxCommandEvent &event);
    void onRemove(wxCommandEvent &event);
    void onEdit(wxCommandEvent &event);
    void onActivateToggle(wxCommandEvent &event);
    void sendFrame();

    BCPanel *m_mainPanel;
    wxStaticText *m_summaryText{};
    std::array<wxStaticText*, BC_MAX_DATA_WORDS> m_dataLabels;
    wxToggleButton *m_activateToggle{};
    FrameConfig m_config;
};