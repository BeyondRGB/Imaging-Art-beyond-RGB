#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <string>
#include <ctime>

namespace btrgb{
    enum TimeType{MILITARY, STANDARD};

    std::string get_date(std::string delim="/");

    std::string get_time(TimeType type, std::string delim=":");
};

#endif // GENERAL_UTILS_H