#pragma once

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Tabs/TabMain.hpp"
#include "wx/notebook.h"

class Tabs : public wxNotebook {
  public:
    Tabs(wxWindow *parent, wxWindowID id,
             const wxPoint &pos = wxDefaultPosition,
             const wxSize &size = wxDefaultSize);
};