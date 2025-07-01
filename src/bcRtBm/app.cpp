// fileName: app.cpp
#include "app.hpp"
#include "mainWindow.hpp"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MainWindow* frame = new MainWindow("AIM Multi-Function Simulator", wxPoint(50, 50), wxSize(1200, 800));
    frame->Show(true);
    return true;
}