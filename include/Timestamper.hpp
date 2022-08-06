#pragma once

#include <ctime>
#include <iomanip>
#include <string>

class Timestamper {
  public:
    static std::string GetCurrent();
};