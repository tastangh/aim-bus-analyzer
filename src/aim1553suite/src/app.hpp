#pragma once
#include <wx/wx.h>

class App : public wxApp {
public:
    bool OnInit() override;
    int OnExit() override; // HATA ÇÖZÜMÜ: Eksik fonksiyon bildirimi eklendi.
};