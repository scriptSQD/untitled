#include <filesystem>
#include <iostream>

#include <ArgParser.hpp>
#include <Logger.hpp>
#include <Utils.hpp>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    bool help = false;

    bool verbose = false;
    bool noStdout = false;

    fs::path filePath{};
    fs::path debugFilePath{};

    // Define stuff based on arguments
    {
        std::unique_ptr<SQD::ArgParser> parser =
            std::make_unique<SQD::ArgParser>(argc, argv);

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

        if (help) {
            Utils::PrintHelp();
            return 0;
        }

        if (!debugFilePath.empty()) {
            SQD::Logger::EnableLogger();
            SQD::Logger::EnableDebugToFile();
            SQD::Logger::SetDebugFile(debugFilePath);

            SQD::Logger::LogToStdout(
                "Log-to-file enabled. Printing output to " +
                fs::absolute(debugFilePath).string());
        }

        if (noStdout) {
            SQD::Logger::EnableNoStdout();
        }

        if (verbose) {
            SQD::Logger::EnableLogger();
            SQD::Logger::LogToStdout(
                "Verbose log enabled. Printing additional output to console");
        }
    }

    if (filePath != "") {
        SQD::Logger::Log("filesystem: Checking " + filePath.string() +
                         " for existence.");
        std::cout << "Checking if path " << filePath
                  << " exists: " << (fs::exists(filePath) ? "Yes" : "No")
                  << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
