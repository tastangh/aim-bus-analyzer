// fileName: ui/ModeSelectionDialog.hpp
#pragma once

#include <wx/wx.h>
#include <vector>

// Seçilen modları MainWindow'a bildirmek için kullanılacak yapı
struct AppConfig {
    bool isMultiMode;
    int singleModeSelection; // 0: BC, 1: RT, 2: BM
    bool bc_selected;
    bool rt_selected;
    bool bm_selected;
};

class ModeSelectionDialog : public wxDialog {
public:
    ModeSelectionDialog(wxWindow* parent, wxWindowID id, const wxString& title);

    // Ana pencerenin, kullanıcının yaptığı seçimi alması için public fonksiyon
    AppConfig GetConfiguration() const { return m_config; }

private:
    void OnModeTypeChange(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);
    void UpdateControlStates();

    wxRadioButton* m_radioSingleMode;
    wxRadioButton* m_radioMultiMode;

    wxRadioBox* m_radioBoxSingleChoice;
    
    wxCheckBox* m_checkBC;
    wxCheckBox* m_checkRT;
    wxCheckBox* m_checkBM;

    wxStaticBoxSizer* m_singleModeSizer;
    wxStaticBoxSizer* m_multiModeSizer;

    AppConfig m_config;
};