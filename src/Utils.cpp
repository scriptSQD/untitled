#include "Utils.hpp"

void Utils::PrintHelp() {
    std::cout
        << R"(untitled - a scriptSQD's C++ program to learn and test new things.

Available arguments and usage patterns:
Usage:
    1. untitled
    2. untitled -v
Arguments:
    --verbose, -v               Enable verbose mode (expanded output).
    --conn-string, --cs         PostgreSQL connection string (may be passed later in GUI).)"
        << std::endl;
}
