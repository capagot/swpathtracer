#ifndef CMD_LINE_OPTIONS_H
#define CMD_LINE_OPTIONS_H

#include <getopt.h>
#include <string>

class CmdLineOptions {
   public:
    static void getOptionValue(int argc, char** argv, const std::string& short_option, int& value) {
        for (int i = 1; i < argc; ++i)
            if (short_option.compare(*(argv + i)) == 0) value = std::atoi(*(argv + i + 1));
    }

    static void getOptionValue(int argc, char** argv, const std::string& short_option, std::string& value) {
        for (int i = 1; i < argc; ++i)
            if (short_option.compare(*(argv + i)) == 0) value = std::string(*(argv + i + 1));
    }
};

#endif  // CMD_LINE_OPTIONS_H
