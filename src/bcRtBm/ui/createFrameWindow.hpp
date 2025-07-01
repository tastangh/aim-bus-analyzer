// fileName: ui/createFrameWindow.hpp
#pragma once

#include "../bc_common.hpp"
#include <wx/wx.h>
#include <wx/combobox.h>
#include <vector>

class BCPanel;
class FrameComponent;

class CreateFrameWindow : public wxFrame {
public:
    explicit CreateFrameWindow(BCPanel *parent);
    explicit CreateFrameWindow(BCPanel *parent, FrameComponent *frameToEdit);

private:
    void createAndLayoutControls();
    void onSave(wxCommandEvent &event);
    void onClose(wxCommandEvent &event);
    void onRandomize(wxCommandEvent &event);
    void onModeChanged(wxCommandEvent &event);
    void onWcChanged(wxCommandEvent &event);
    void populateFieldsFromConfig(const FrameConfig &config);
    FrameConfig buildConfigFromFields();
    void updateControlStates();

    BCPanel *m_parentPanel;
    FrameComponent *m_editingFrame = nullptr;
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
    wxStaticText* m_saLabel{};
};