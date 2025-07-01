// fileName: bm_common.hpp
#pragma once

#include <wx/wx.h>      // DÜZELTME: Bu satır eklendi.
#include "Api1553.h" // AiReturn gibi tipler için

// BM'yi başlatmak için kullanılacak konfigürasyon yapısı
struct ConfigBmUi {
    AiUInt32 ulDevice;
    AiUInt32 ulStream;
    AiUInt32 ulCoupling;
};

// Arayüzdeki kontrollere atanacak ID'ler
enum {
  ID_ADD_MENU = wxID_HIGHEST + 1,
  ID_ADD_BTN,
  ID_FILTER_BTN,
  ID_CLEAR_BTN,
  ID_FILTER_MENU,
  ID_CLEAR_MENU,
  ID_DEVICE_ID_TXT,
  ID_RT_SA_TREE,
  ID_LOG_TO_FILE_CHECKBOX
};

// Üst bardaki bileşenlerin standart yüksekliği
constexpr int TOP_BAR_COMP_HEIGHT = 28;