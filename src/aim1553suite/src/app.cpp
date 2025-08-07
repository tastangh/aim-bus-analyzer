#include "app.hpp"
#include "ui/MainFrame.hpp" // MainFrame'i bilmesi için

// Bu makro 'main' fonksiyonunu oluşturur!
wxIMPLEMENT_APP(App);

bool App::OnInit() {
    if (!wxApp::OnInit()) {
        return false;
    }
    
    auto *frame = new MainFrame();
    frame->Show(true);
    SetTopWindow(frame);
    
    return true;
}