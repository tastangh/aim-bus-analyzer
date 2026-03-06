#pragma once

#include "common.hpp"
#include <wx/frame.h> // Bu sınıf bir wxFrame'dir, yani kendi başına bir penceredir.
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <vector>

// Forward declarations
class BusControllerPanel; // DEĞİŞİKLİK: Artık BusControllerFrame yerine BusControllerPanel'i tanıyoruz.
class FrameComponent;
class wxBoxSizer;

class FrameCreationFrame : public wxFrame {
public:
    // Kurucu fonksiyon artık parent olarak bir BusControllerPanel alıyor.
    explicit FrameCreationFrame(BusControllerPanel *parent);
    
    // Düzenleme modu için olan kurucu fonksiyon da aynı şekilde güncellendi.
    explicit FrameCreationFrame(BusControllerPanel *parent, FrameComponent *frameToEdit);

private:
    // Fonksiyon bildirimleri aynı kalır.
    void createAndLayoutControls();
    void onSave(wxCommandEvent &event);
    void onWcChanged(wxCommandEvent &event);
    void onModeChanged(wxCommandEvent &event);
    void onRandomize(wxCommandEvent &event);
    void onClose(wxCommandEvent &event);
    void populateFieldsFromConfig(const FrameConfig &config);
    FrameConfig buildConfigFromFields();
    void updateControlStates();

    // Üye Değişkenler
    // DEĞİŞİKLİK: m_parentFrame, m_parentPanel olarak değiştirildi ve türü güncellendi.
    // Bu, bu pencerenin "Save" butonuna basıldığında doğru panele veri göndermesini sağlar.
    BusControllerPanel *m_parentPanel; 
    
    FrameComponent *m_editingFrame = nullptr;
    
    // Arayüz elemanları için işaretçiler (bunlar aynı kalır)
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