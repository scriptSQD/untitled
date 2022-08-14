#pragma once

#include "wx/font.h"

class UntitledFonts {
  public:
    inline static const auto FONT_H1 =
        wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    inline static const auto FONT_H2 = wxFont(
        15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM);
    inline static const auto FONT_REGULAR = wxFont(
        12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
};
