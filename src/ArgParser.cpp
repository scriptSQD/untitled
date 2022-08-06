#include <ArgParser.hpp>
#include <algorithm>

ArgParser::ArgParser(int argc, char *argv[]) {
    this->argc = argc;
    if (this->argc < 2) { return; }

    this->argv = std::vector<std::string>(argv, argc+argv);
}

size_t ArgParser::findIndex(std::vector<std::string>& searchSource, const std::vector<std::string>& flags) {
    for (auto & flag : flags) {
        auto it = std::find(searchSource.begin(), searchSource.end(), flag);
        if (it != searchSource.end()) {
            return it - searchSource.begin();
        }
    }

    return -1;
}

ArgParser *ArgParser::IfFlagsSet(const std::vector<std::string> &flags) {
    const size_t flagIndex = ArgParser::findIndex(this->argv, flags);
    if (flagIndex == -1) {
        this->lastParsedFlagIndex = -1;
        return this;
    }

    this->lastParsedFlagIndex = flagIndex;
    return this;
}

std::string ArgParser::parseByIndex(size_t &flagIndex) {
    if (flagIndex == -1)
        return "";

    if (this->argv.size() >= flagIndex + 1) {
        return this->argv.at(flagIndex + 1);
    }

    return "";
}
