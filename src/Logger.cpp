#include <Logger.hpp>

const std::string Logger::COLOR_GRAY = "\033[37m";
const std::string Logger::COLOR_ESCAPE = "\033[0m";

Logger::Logger() = default;
Logger::Logger(fs::path &debugFilePath) {
    Logger::debugFile = {};
    Logger::InitDebugToFile(debugFilePath);
}

void Logger::InitDebugToFile(const fs::path &path) {
    try {
        Logger::debugFile.open(path, OpenMode);
    } catch (std::ofstream::failure &error) {
        Logger::Handle("Failed to open file at " + path.string() +
                           ".\nError output: " + error.what(),
                       error);
    }
}

void Logger::SetDebugFile(const fs::path &path) {
    if (!Logger::debugFile.is_open()) {
        Logger::debugFile.open(path, OpenMode);
    } else {
        Logger::debugFile.close();
        Logger::SetDebugFile(path);
    }
}

Logger::~Logger() {
    // Close all streams and stuff
    // that possibly causes headaches
    Logger::debugFile.close();
}

void Logger::EnableLogger(bool enable) { Logger::LoggerEnabled = enable; }
void Logger::EnableDebugToFile(bool enable) { Logger::ToFile = enable; }
void Logger::EnableNoStdout(bool enable) { Logger::NoStdout = enable; }

void Logger::DisableLogger(bool disable) { Logger::LoggerEnabled = !disable; }
void Logger::DisableDebugToFile(bool disable) { Logger::ToFile = !disable; }
void Logger::DisableNoStdout(bool disable) { Logger::NoStdout = !disable; }

void Logger::Handle(const std::string &message, std::ofstream::failure &error) {
    Logger::LogToStdout(message, "ERROR");
    throw error;
}

void Logger::Log(const std::string &message, const std::string &prefix) {
    if (!Logger::LoggerEnabled) {
        return;
    }

    if (Logger::ToFile) {
        Logger::LogToFile(message, prefix);
    } else {
        Logger::LogToStdout(message, prefix);
    }
}

void Logger::LogToStdout(const std::string &message,
                         const std::string &prefix) {
    std::cout << COLOR_GRAY << Timestamper::GetCurrent() << " [" << prefix
              << "]: " << message << COLOR_ESCAPE << std::endl;
}

void Logger::LogToFile(const std::string &message, const std::string &prefix) {
    try {
        Logger::debugFile << Timestamper::GetCurrent() << " [" << prefix
                          << "]: " << message << std::endl;
    } catch (std::ofstream::failure &error) {
        Logger::Handle(static_cast<std::string>(
                           "Failed to write to debug file.\nError output: ")
                           .append(error.what()),
                       error);
    }

    if (!Logger::NoStdout) {
        Logger::LogToStdout(message, prefix);
    }
}
