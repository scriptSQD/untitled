#pragma once

#include <string>
#include <vector>

class ArgParser {
  private:
    int argc;
    std::vector<std::string> argv;
    size_t lastParsedFlagIndex = -1;

    static size_t findIndex(std::vector<std::string> &,
                            const std::vector<std::string> &);

    std::string parseByIndex(size_t &flagIndex);

  public:
    /**
     * Initialize ArgParser with argument count and argument vector.
     * @param argc Argument count.
     * @param argv Argument vector.
     */
    ArgParser(int argc, char *argv[]);

    /**
     * Checks whether argv has any of the passed flags.
     * Sets internal bit to either the index of flag in argv or -1 if not found.
     * Then you can call GetFlagValue to get the value to preceding flag.
     * @param flags
     * @return Pointer to 'this' to allow easy method chaining.
     */
    ArgParser *IfFlagsSet(const std::vector<std::string> &flags);

    /**
     * Parses flag value to a given variable reference based on internal bit
     * (last parsed flag, check out ArgParser::IfFlagsSet).
     * @tparam T Type for the variable to where write the flags value. Should
     * ideally be std::string or something that is easily casted to std::string.
     * @param dest A variable reference to where to write the flags value.
     * @return Pointer to 'this' to allow easy method chaining.
     */
    template <typename T> ArgParser *GetFlagValue(T &dest) {
        dest = parseByIndex(this->lastParsedFlagIndex);
        return this;
    }

    /**
     * Template specialization for boolean, when you just need to know whether
     * the flag has been to, not its value.
     * @param dest A variable reference to where to write the flags value.
     * @return Pointer to 'this' to allow easy method chaining.
     */
    template <> ArgParser *GetFlagValue<bool>(bool &dest) {
        dest = this->lastParsedFlagIndex != -1;
        return this;
    }
};