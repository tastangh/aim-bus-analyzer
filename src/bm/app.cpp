#include "app.hpp"
#include "mainWindow.hpp"
#include "bm.hpp" // BM sınıfını ve ApiExit'i kullanabilmek için

wxIMPLEMENT_APP(BusMonitorApp);

bool BusMonitorApp::OnInit() {
  if (!wxApp::OnInit()) return false; // Temel wxApp başlatmasını yap

  auto *frame = new BusMonitorFrame();
  frame->Show(true);
  SetTopWindow(frame); // Ana pencereyi ayarla
  return true;
}

int BusMonitorApp::OnExit() {
    // BM singleton'ının destructor'ı normalde ApiExit'i çağıracak şekilde ayarlanabilir.
    // Eğer BM sınıfının destructor'ında ApiExit yoksa veya emin olmak için:
    printf("BusMonitorApp::OnExit - Calling ApiExit if not already called.\n");
    // BM::getInstance().stop(); // Eğer hala çalışıyorsa durdur (frame kapanırken zaten olmalı)
    // ApiExit(); // BM destructor'ı bunu yapmıyorsa burada çağırın. BM destructor'ı yapıyorsa bu gereksiz.
                // BM singleton olduğu için program biterken destructor'ı otomatik çağrılır.
    return wxApp::OnExit();
}