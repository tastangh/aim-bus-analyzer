// fileName: app.cpp
#include "app.hpp"
#include "mainWindow.hpp"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Ana pencereyi oluştur ve göster
    auto *frame = new MainWindow();
    frame->Show(true);
    return true;
}