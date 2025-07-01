// fileName: ui/ModeSelectionDialog.hpp
#pragma once

#include <wx/wx.h>
#include <vector>

// Hangi ana modun seçildiğini belirtmek için enum
enum class OperationMode {
    SINGLE,
    CUSTOM_MULTI,
    SCENARIO_BC_RT_BM
};

// Seçilen modları MainWindow'a bildirmek için kullanılacak yapı
struct AppConfig {
    OperationMode mainMode;
    int singleModeSelection; // 0: BC, 1: RT, 2: BM
    bool bc_selected;
    bool rt_selected;
    bool bm_selected;
};

class ModeSelectionDialog : public wxDialog {
public:
    ModeSelectionDialog(wxWindow* parent, wxWindowID id, const wxString& title);

    AppConfig GetConfiguration() const { return m_config; }

private:
    void OnMainModeChange(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);
    void UpdateControlStates();

    wxRadioBox* m_mainModeChooser;
    
    wxStaticBoxSizer* m_singleModeSizer;
    wxRadioBox* m_radioBoxSingleChoice;
    
    wxStaticBoxSizer* m_multiModeSizer;
    wxCheckBox* m_checkBC;
    wxCheckBox* m_checkRT;
    wxCheckBox* m_checkBM;

    wxStaticBoxSizer* m_scenarioModeSizer;

    AppConfig m_config;
};