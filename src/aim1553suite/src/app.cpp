#include "app.hpp"
#include "ui/MainFrame.hpp"
#include <wx/msgdlg.h>

// Api1553.h ve ApiInit/ApiExit çağrıları buradan kaldırıldı.

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    if (!wxApp::OnInit()) {
        return false;
    }
    
    // ApiInit() kontrolü artık burada değil.
    
    auto *frame = new MainFrame();
    frame->Show(true);
    SetTopWindow(frame);
    
    return true;
}

int App::OnExit() {
    // ApiExit() çağrısı da buradan kaldırıldı.
    // Her panel kendi işi bittiğinde temizliğini yapacak.
    return wxApp::OnExit();
}