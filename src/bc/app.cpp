#include "app.hpp"
#include "mainWindow.hpp"

wxIMPLEMENT_APP(BusControllerApp);

bool BusControllerApp::OnInit() {
  auto *frame = new BusControllerFrame();
  frame->Show(true);
  return true;
}