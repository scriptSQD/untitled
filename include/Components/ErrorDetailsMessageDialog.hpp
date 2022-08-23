#pragma once

#include "UntitledPrecomp.hpp"

class ErrorDetailsMessageDialog : public wxMessageDialog {
  public:
    ErrorDetailsMessageDialog(const std::string &title,
                              const std::string &description,
                              const std::string &error);
};