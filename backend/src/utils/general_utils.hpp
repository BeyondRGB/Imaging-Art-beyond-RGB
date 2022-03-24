#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <string>
#include <ctime>

namespace btrgb{
    enum TimeType{MILITARY, STANDARD};

    /**
     * @brief Creates a date string in the form
     *      year<delim>month<delim>day
     * 
     * @param delim the desired delimiter to use between the items of the date
     * @return std::string 
     */
    std::string get_date(std::string delim="/");

    /**
     * @brief Creates a time string in the form
     *      hr<delim>min<delim>sec
     * 
     * @param type identifies if time should be in Military or Standard time
     * @param delim the desired delimiter to use between the items of the time
     * @return std::string 
     */
    std::string get_time(TimeType type, std::string delim=":");
};

#endif // GENERAL_UTILS_H