#include "Tabs/Tabs.hpp"

Tabs::Tabs(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                   const wxSize &size)
    : wxNotebook(parent, id, pos, size) {
    auto *main_tab = new TabMain(this, wxID_ANY);
    this->AddPage(main_tab, "Home");
}
