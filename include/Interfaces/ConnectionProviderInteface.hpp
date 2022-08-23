#pragma once

#include <string>

class ConnectionInfoProvider {
  public:
    ConnectionInfoProvider() = default;

    [[nodiscard]] virtual std::string GetConnectionString() const = 0;
    [[nodiscard]] virtual std::string GetConnectionDisplayName() const = 0;

    virtual void ClearInput() = 0;
};