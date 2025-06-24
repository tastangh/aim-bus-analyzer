#include "app.hpp"
#include "ui/mainWindow.hpp"

wxIMPLEMENT_APP(BusControllerApp);

bool BusControllerApp::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  auto *frame = new BusControllerFrame();
  frame->Show(true);
  SetTopWindow(frame);
  return true;
}