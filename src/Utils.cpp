#include "Utils.hpp"

void Utils::printHelp() {
    std::cout << R"(untitled - a scriptSQD's C++ program to learn and test new things.

Available arguments and usage patterns:
Usage:
    1. untitled
    2. untitled -v
    3. untitled -f /bin
    4. untitled --df ~/untitled_log_2022.log
Arguments:
    --file, -f                  File path to check for existence.
    --verbose, -v               Enable verbose mode (expanded output).
    --help, -h                  Display help menu and exit.
    --debug-to-file, --df       Print debug info to file specified file
    --no-stdout, -n             Do not print debug to stdout if --debug-to-file is enabled.)";
}
