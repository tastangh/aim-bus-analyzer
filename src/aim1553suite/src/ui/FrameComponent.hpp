#pragma once

#include "common.hpp"
#include "AiOs.h"       // AIM donanım kütüphanesinden tipler için
#include <wx/panel.h>   // Bu sınıf zaten bir wxPanel olduğu için temel sınıf aynı kalır
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <array>

// Forward declarations
class BusControllerPanel; // DEĞİŞİKLİK: BusControllerFrame yerine BusControllerPanel'i kullanacağız.

// Bu sınıf, bir 1553 frame'ini görsel olarak temsil eden paneldir.
// İçinde özet bilgi, veri kelimeleri ve kontrol butonları bulunur.
class FrameComponent : public wxPanel {
public:
    // Kurucu fonksiyon, ebeveyn olarak bir wxWindow (bizim durumumuzda wxScrolledWindow) alır.
    explicit FrameComponent(wxWindow *parent, const FrameConfig &config);

    // Bu bileşenin public arayüzü, yani dışarıdan çağrılabilen metotları.
    void updateValues(const FrameConfig &config);
    void sendFrame();
    bool isActive() const;
    const FrameConfig &getFrameConfig() const { return m_config; }
    void updateDataUI(const std::array<AiUInt16, BC_MAX_DATA_WORDS> &newData);

    // AIM donanım kaynak ID'lerini ayarlamak ve almak için metotlar.
    void setAimIds(AiUInt16 xferId, AiUInt16 hdrId, AiUInt16 bufId);
    AiUInt16 getAimTransferId() const { return m_aimTransferId; }
    AiUInt16 getAimHeaderId() const { return m_aimHeaderId; }
    AiUInt16 getAimBufferId() const { return m_aimBufferId; }

private:
    // Bu paneldeki butonların olay yöneticileri.
    void onSend(wxCommandEvent &event);
    void onRemove(wxCommandEvent &event);
    void onEdit(wxCommandEvent &event);
    void onActivateToggle(wxCommandEvent &event);

    // Üye Değişkenler
    // DEĞİŞİKLİK: m_mainWindow, m_parentPanel olarak değiştirildi ve türü güncellendi.
    // Bu FrameComponent'in, kendisini içeren BusControllerPanel'e erişmesini sağlar.
    BusControllerPanel *m_parentPanel;

    // Arayüz elemanları ve veri üyeleri.
    wxStaticText *m_summaryText{};
    std::array<wxStaticText*, BC_MAX_DATA_WORDS> m_dataLabels;
    wxToggleButton *m_activateToggle{};
    FrameConfig m_config;

    // AIM donanım kaynak ID'leri.
    AiUInt16 m_aimTransferId;
    AiUInt16 m_aimHeaderId;
    AiUInt16 m_aimBufferId;
};