// fileName: ui/RTPanel.cpp
#include "RTPanel.hpp"
RTPanel::RTPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    SetBackgroundColour(*wxLIGHT_GREY);
    wxStaticText* text = new wxStaticText(this, wxID_ANY, "Remote Terminal (RT) Panel - UI TO BE IMPLEMENTED", wxPoint(20, 20));
    wxFont font = text->GetFont(); font.SetPointSize(14); font.SetWeight(wxFONTWEIGHT_BOLD); text->SetFont(font);
}