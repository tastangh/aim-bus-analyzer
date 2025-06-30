// fileName: app.hpp
#pragma once

#include <wx/wx.h>
#include <string>
#include <array>
#include "AiOs.h"


struct FrameConfig {
    std::string label;
    char bus;
    int rt;
    int sa;
    int wc;
    bool enabled;
    std::array<std::string, 32> data;
};

// Ana uygulama sınıfı
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};