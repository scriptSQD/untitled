#include "Utils.hpp"

void Utils::printHelp() {
    const std::string helpOutput = "Available arguments and usage patterns:\n"
"Usage:\n"
"\t1. untitled\n"
"\t2. untitled -v\n"
"\t3. untitled -f /bin\n"
"\t4. untitled --df ~/untitled_log_2022.log\n\n"
"Arguments:\n"
"\t--file, -f\t\t\tFile path to check for existence.\n"
"\t--verbose, -v\t\t\tEnable verbose mode (expanded output).\n"
"\t--help, -h\t\t\tDisplay help menu and exit.\n"
"\t--debug-to-file, --df\t\tPrint debug info to file specified file\n"
"\t--no-stdout, -n\t\t\tDo not print debug to stdout if --debug-to-file is enabled.\n";

    std::cout << helpOutput << std::endl;
}
