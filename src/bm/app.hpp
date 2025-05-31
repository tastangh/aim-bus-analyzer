#pragma once

#include <wx/wx.h>

class BusMonitorApp : public wxApp {
public:
  bool OnInit() override;
  int OnExit() override; 
};