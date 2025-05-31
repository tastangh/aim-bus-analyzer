#include "app.hpp"
#include "mainWindow.hpp"
#include "bm.hpp" 

wxIMPLEMENT_APP(BusMonitorApp);

bool BusMonitorApp::OnInit() {
  if (!wxApp::OnInit()) return false;

  auto *frame = new BusMonitorFrame();
  frame->Show(true);
  SetTopWindow(frame);
  return true;
}

int BusMonitorApp::OnExit() {
    printf("BusMonitorApp::OnExit - Calling ApiExit if not already called.\n");
    return wxApp::OnExit();
}