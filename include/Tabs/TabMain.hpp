#pragma once

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class TabMain : public wxWindow {
  public:
    TabMain(wxWindow *parent, wxWindowID id,
            const wxPoint &pos = wxDefaultPosition,
            const wxSize &size = wxDefaultSize);
};