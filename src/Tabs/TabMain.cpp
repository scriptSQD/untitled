#include "Tabs/TabMain.hpp"
TabMain::TabMain(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                 const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    new wxStaticText(this, wxID_ANY, "Hello! This is the main tab!");
}
