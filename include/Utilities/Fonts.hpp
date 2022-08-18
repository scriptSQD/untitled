#pragma once

#include "wx/font.h"

class UntitledFonts {
  public:
    inline static const auto FONT_H1 =
        wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD);
    inline static const auto FONT_H2 = wxFont(
        13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM);
    inline static const auto FONT_REGULAR = wxFont(
        11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
};
