#include <filesystem>
#include <iostream>

#include <ArgParser.hpp>
#include <Logger.hpp>
#include <Utils.hpp>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    bool help;

    bool verbose;
    bool noStdout;

    fs::path filePath;
    fs::path debugFilePath;

    auto *parser = new ArgParser(argc, argv);

    parser->IfFlagsSet({"--help", "-h"})
        ->GetFlagValue(help)
        ->IfFlagsSet({"--file", "-f"})
        ->GetFlagValue(filePath)
        ->IfFlagsSet({"--verbose", "-v"})
        ->GetFlagValue(verbose)
        ->IfFlagsSet({"--debug-to-file", "--df"})
        ->GetFlagValue(debugFilePath)
        ->IfFlagsSet({"--no-stdout", "-n"})
        ->GetFlagValue(noStdout);

    // Define stuff based on arguments
    {
        if (help) {
            Utils::printHelp();
            return 0;
        }

        if (!debugFilePath.empty()) {
            Logger::EnableLogger();
            Logger::EnableDebugToFile();
            Logger::SetDebugFile(debugFilePath);

            Logger::LogToStdout("Log-to-file enabled. Printing output to " +
                                debugFilePath.string());
        }

        if (noStdout) {
            Logger::EnableNoStdout();
        }

        if (verbose) {
            Logger::EnableLogger();
            Logger::LogToStdout(
                "Verbose log enabled. Printing additional output to console");
        }
    }

    if (filePath != "") {
        Logger::Log("filesystem: Checking " + filePath.string() +
                    " for existence.");
        std::cout << "Checking if path " << filePath
                  << " exists: " << (fs::exists(filePath) ? "Yes" : "No")
                  << std::endl;
    }

    Logger::SetDebugFile("/home/sqd/untitled_log_new.log");
    Logger::Log("logger: Changed debug file to ~/untitled_log_new.log");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
