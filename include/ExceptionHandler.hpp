#pragma once

#include <UntitledPrecomp.hpp>

#include <exception>

class ExceptionHandler {
  public:
    static std::string HandleEptr(std::exception_ptr e);
};