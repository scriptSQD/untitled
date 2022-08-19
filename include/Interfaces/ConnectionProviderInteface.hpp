#pragma once

#include <string>

class IConnectionStringProvider {
  public:
    IConnectionStringProvider() = default;

    [[nodiscard]] virtual std::string GetConnectionString() const = 0;
    virtual void ClearInput() = 0;
};