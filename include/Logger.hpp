#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <Timestamper.hpp>

namespace fs = std::filesystem;

class Logger {
  public:
    Logger();
    explicit Logger(fs::path &debugFilePath);
    ~Logger();

    /**
     * Initializes debug file to write into.
     * @param path Opens std::ofstream at the given path for write.
     */
    static void InitDebugToFile(const fs::path &path);


    // Syntactic sugar
    /**
     * Syntactic sugar to set logger state to enabled.
     * @param enable Whether to enable logger.
     */
    static void EnableLogger(bool enable = true);
    /**
     * Syntactic sugar to set logger state to disabled.
     * @param disable Whether to disable logger.
     */
    static void DisableLogger(bool disable = true);

    /**
     * Syntactic sugar to enable writing debug info to file. You should also set
     * debug file if not already, so that logger has somewhere to write to.
     * @param enable Whether to enable debugging to file.
     */
    static void EnableDebugToFile(bool enable = true);
    /**
     * Syntactic sugar to enable writing debug info to file. You should also set
     * debug file if not already, so that logger has somewhere to write to.
     * @param disable
     */
    static void DisableDebugToFile(bool disable = true);

    /**
     * Do not mirror debug log to stdout if debugging to file.
     * @param enable Whether to enable no stdout.
     */
    static void EnableNoStdout(bool enable = true);
    /**
     * Do not mirror debug log to stdout if debugging to file.
     * @param disable Whether to disable no stdout.
     */
    static void DisableNoStdout(bool disable = true);

    /**
     * Opens given path as current debug file.
     * All subsequent logs will be written to the file at given path.
     * @param path Opens std::ofstream at the given path for write.
     */
    static void SetDebugFile(const fs::path &path);

    /**
     * Log method uses class' internal state
     * to log either to file, to stdout or both.
     *
     * @param message Gets printed to the logging destination.
     * @param prefix Is optional, this is appended as logging level (like LOG,
     * WARNING, ERROR, etc.)
     */
    static void Log(const std::string &message,
                    const std::string &prefix = "LOG");

    static void LogToStdout(const std::string &message,
                            const std::string &prefix = "LOG");

  private:
    static void Handle(const std::string &message,
                       std::ofstream::failure &error);

    static void LogToFile(const std::string &message,
                          const std::string &prefix = "LOG");

    inline static bool ToFile = false;
    inline static bool NoStdout = false;
    inline static bool LoggerEnabled = false;

    static const std::string COLOR_RED;
    static const std::string COLOR_YELLOW;
    static const std::string COLOR_GRAY;
    static const std::string COLOR_ESCAPE;

    static const auto OpenMode =
        std::ios::out | std::ios::binary | std::ios::app;
    inline static std::ofstream debugFile{};
};
