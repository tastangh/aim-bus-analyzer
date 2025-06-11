#include "createFrameWindow.hpp"

#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/wx.h>

#include "common.hpp"
#include "mainWindow.hpp"

FrameCreationFrame::FrameCreationFrame(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "Create 1553 Frame"), m_parent(parent) {
  createFrame();

  m_saveButton->Bind(wxEVT_BUTTON, &FrameCreationFrame::onSaveAdd, this);

  wxCommandEvent emptyEvent(wxEVT_COMBOBOX, GetId());
  onModeChanged(emptyEvent);
  onWcChanged(emptyEvent);
}

FrameCreationFrame::FrameCreationFrame(wxWindow *parent, FrameComponent *frame)
    : wxFrame(parent, wxID_ANY, "Editing 1553 Frame"), m_parent(parent) {
  createFrame();

  m_labelTextCtrl->SetValue(frame->getLabel());
  m_busCombo->SetValue(wxString::FromAscii(frame->getBus()));
  m_rtCombo->SetValue(std::to_string(frame->getRt()));
  m_rt2Combo->SetValue(std::to_string(frame->getRt2()));
  m_saCombo->SetValue(std::to_string(frame->getSa()));
  m_sa2Combo->SetValue(std::to_string(frame->getSa2()));
  m_wcCombo->SetValue(std::to_string(frame->getWc()));
  
  // Düzeltme: enum class değerini int'e dönüştürerek ata
  m_modeCombo->SetSelection(static_cast<int>(frame->getMode()));

  const auto& frameData = frame->getData();
  for (size_t i = 0; i < m_dataTextCtrls.size(); ++i) {
    if (i < frameData.size()) {
        m_dataTextCtrls.at(i)->SetValue(frameData.at(i));
    }
  }

  m_saveButton->Bind(wxEVT_BUTTON, [this, frame](wxCommandEvent &event) { onSaveEdit(event, frame); });

  wxCommandEvent emptyEvent(wxEVT_COMBOBOX, GetId());
  onModeChanged(emptyEvent);
  onWcChanged(emptyEvent);
}

void FrameCreationFrame::createFrame() {
  m_mainSizer = new wxBoxSizer(wxVERTICAL);
  auto *topSizer = new wxBoxSizer(wxHORIZONTAL);
  m_cmdWord2Sizer = new wxBoxSizer(wxHORIZONTAL);
  auto *middleSizer = new wxBoxSizer(wxHORIZONTAL);
  auto *dataGridSizer = new wxGridSizer(4, 8, 5, 5); // 4 satır, 8 sütun
  auto *labelSizer = new wxBoxSizer(wxHORIZONTAL);
  auto *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

  m_saveButton = new wxButton(this, wxID_ANY, "Save Frame", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
  auto *closeButton = new wxButton(this, wxID_ANY, "Cancel", wxDefaultPosition, wxSize(100, TOP_BAR_COMP_HEIGHT));
  auto *randomizeButton = new wxButton(this, wxID_ANY, "Randomize Data", wxDefaultPosition, wxSize(120, TOP_BAR_COMP_HEIGHT));

  wxArrayString rtSaWcOptions;
  for(int i=0; i<32; ++i) rtSaWcOptions.Add(std::to_string(i));

  wxString busOptions[] = {"A", "B"};
  wxString modeOptions[] = {"BC->RT", "RT->BC", "RT->RT"};

  auto *busLabel = new wxStaticText(this, wxID_ANY, "Bus: ");
  m_busCombo = new wxComboBox(this, wxID_ANY, "A", wxDefaultPosition, wxDefaultSize, 2, busOptions, wxCB_READONLY);

  auto *rtLabel = new wxStaticText(this, wxID_ANY, "RT: ");
  m_rtCombo = new wxComboBox(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);

  auto *rt2Label = new wxStaticText(this, wxID_ANY, "RT RX: ");
  m_rt2Combo = new wxComboBox(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);

  auto *saLabel = new wxStaticText(this, wxID_ANY, "SA: ");
  m_saCombo = new wxComboBox(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);

  auto *sa2Label = new wxStaticText(this, wxID_ANY, "SA RX: ");
  m_sa2Combo = new wxComboBox(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);

  auto *wcLabel = new wxStaticText(this, wxID_ANY, "WC: ");
  m_wcCombo = new wxComboBox(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, rtSaWcOptions, wxCB_READONLY);

  auto *modeLabel = new wxStaticText(this, wxID_ANY, "Mode: ");
  m_modeCombo = new wxComboBox(this, wxID_ANY, "BC->RT", wxDefaultPosition, wxDefaultSize, 3, modeOptions, wxCB_READONLY);

  auto *dataLabel = new wxStaticText(this, wxID_ANY, "Data (Hex): ");
  m_labelTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
  m_labelTextCtrl->SetHint("Set frame label");

  for (int i = 0; i < RT_SA_MAX_COUNT; ++i) {
    auto *data = new wxTextCtrl(this, wxID_ANY, "0000", wxDefaultPosition, wxSize(70, TOP_BAR_COMP_HEIGHT));
    m_dataTextCtrls.push_back(data);
    dataGridSizer->Add(data, 0, wxEXPAND);
  }

  // Layout
  topSizer->Add(busLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_busCombo, 0, wxRIGHT, 10);
  topSizer->Add(rtLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_rtCombo, 0, wxRIGHT, 10);
  topSizer->Add(saLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_saCombo, 0, wxRIGHT, 10);
  topSizer->Add(wcLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_wcCombo, 0, wxRIGHT, 10);
  topSizer->Add(modeLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); topSizer->Add(m_modeCombo, 0, wxRIGHT, 10);
  
  m_cmdWord2Sizer->Add(rt2Label, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); m_cmdWord2Sizer->Add(m_rt2Combo, 0, wxRIGHT, 10);
  m_cmdWord2Sizer->Add(sa2Label, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5); m_cmdWord2Sizer->Add(m_sa2Combo, 0, wxRIGHT, 10);

  middleSizer->Add(dataLabel, 0, wxALIGN_CENTER_VERTICAL, 5);
  labelSizer->Add(m_labelTextCtrl, 1, wxEXPAND | wxALL, 5);
  bottomSizer->Add(randomizeButton, 0, wxALL, 5);
  bottomSizer->AddStretchSpacer();
  bottomSizer->Add(closeButton, 0, wxALL, 5);
  bottomSizer->Add(m_saveButton, 0, wxALL, 5);

  m_mainSizer->Add(topSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  m_mainSizer->Add(m_cmdWord2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  m_mainSizer->Add(middleSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
  m_mainSizer->Add(dataGridSizer, 0, wxEXPAND | wxALL, 10);
  m_mainSizer->Add(labelSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
  m_mainSizer->AddStretchSpacer();
  m_mainSizer->Add(bottomSizer, 0, wxEXPAND | wxALL, 5);

  SetSizerAndFit(m_mainSizer);
  Centre();
  
  // Olayları bağlama
  m_wcCombo->Bind(wxEVT_COMBOBOX, &FrameCreationFrame::onWcChanged, this);
  m_modeCombo->Bind(wxEVT_COMBOBOX, &FrameCreationFrame::onModeChanged, this);
  randomizeButton->Bind(wxEVT_BUTTON, &FrameCreationFrame::onRandomize, this);
  closeButton->Bind(wxEVT_BUTTON, &FrameCreationFrame::onClose, this);
}

void FrameCreationFrame::onSaveAdd(wxCommandEvent & /*event*/) {
  auto *parentFrame = dynamic_cast<BusControllerFrame *>(m_parent);
  if (!parentFrame) return;

  std::array<std::string, RT_SA_MAX_COUNT> data;
  for (size_t i = 0; i < data.size(); ++i) {
    data.at(i) = m_dataTextCtrls.at(i)->GetValue().ToStdString();
  }

  std::string label = m_labelTextCtrl->GetValue().ToStdString();
  parentFrame->addFrameToList(
      label.empty() ? "Untitled Frame" : label, 
      m_busCombo->GetValue().ToStdString()[0],
      wxAtoi(m_rtCombo->GetValue()), wxAtoi(m_rt2Combo->GetValue()),
      wxAtoi(m_saCombo->GetValue()), wxAtoi(m_sa2Combo->GetValue()),
      wxAtoi(m_wcCombo->GetValue()), 
      static_cast<BcMode>(m_modeCombo->GetSelection()), 
      data
  );
  Close(true);
}

void FrameCreationFrame::onSaveEdit(wxCommandEvent & /*event*/, FrameComponent *frame) {
  std::array<std::string, RT_SA_MAX_COUNT> data;
  for (size_t i = 0; i < data.size(); ++i) {
    data.at(i) = m_dataTextCtrls.at(i)->GetValue().ToStdString();
  }
  std::string label = m_labelTextCtrl->GetValue().ToStdString();

  frame->updateValues(
      label.empty() ? "Untitled Frame" : label, 
      m_busCombo->GetValue().ToStdString()[0], 
      wxAtoi(m_rtCombo->GetValue()), wxAtoi(m_rt2Combo->GetValue()),
      wxAtoi(m_saCombo->GetValue()), wxAtoi(m_sa2Combo->GetValue()),
      wxAtoi(m_wcCombo->GetValue()), 
      static_cast<BcMode>(m_modeCombo->GetSelection()), 
      data
  );
  Close(true);
}


void FrameCreationFrame::onWcChanged(wxCommandEvent & /*event*/) {
  int wc = wxAtoi(m_wcCombo->GetValue());
  if (wc == 0) wc = 32;

  bool dataEnabled = (static_cast<BcMode>(m_modeCombo->GetSelection()) == BcMode::BC_TO_RT);
  for (size_t i = 0; i < m_dataTextCtrls.size(); ++i) {
    m_dataTextCtrls[i]->Enable(dataEnabled && (i < (size_t)wc));
  }
}

void FrameCreationFrame::onModeChanged(wxCommandEvent & /*event*/) {
  bool isRtToRt = (m_modeCombo->GetSelection() == static_cast<int>(BcMode::RT_TO_RT));
  m_cmdWord2Sizer->ShowItems(isRtToRt);
  
  // onWcChanged'i çağırmak yerine, bir event oluşturup gönder
  wxCommandEvent wcEvent(wxEVT_COMMAND_COMBOBOX_SELECTED, m_wcCombo->GetId());
  wxPostEvent(this, wcEvent);

  m_mainSizer->Layout();
  this->Fit();
}

void FrameCreationFrame::onRandomize(wxCommandEvent & /*event*/) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 65535);

  for (auto &dataTextCtrl : m_dataTextCtrls) {
    std::stringstream ss;
    ss << std::uppercase << std::setw(4) << std::setfill('0') << std::hex << distr(eng);
    dataTextCtrl->SetValue(ss.str());
  }
}

void FrameCreationFrame::onClose(wxCommandEvent & /*event*/) { Close(true); }