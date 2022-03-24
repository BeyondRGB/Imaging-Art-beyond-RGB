#include "utils/general_utils.hpp"

std::string btrgb::get_date(std::string delim){
    std::time_t now = std::time(0);
    std::tm *ltm = std::localtime(&now);

    std::string year = std::to_string(1900+ltm->tm_year);
    std::string month = std::to_string(1+ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);
    if(ltm->tm_mday < 10)
        day  = "0" + day;
    if(ltm->tm_mon < 10)
        month = "0" + month;
    return year + delim + month + delim + day;
}

std::string btrgb::get_time(TimeType type, std::string delim){
    int hr_offset = 0;
    if(type == TimeType::MILITARY)
        hr_offset = 5;
    std::time_t now = std::time(0);
    std::tm *ltm = std::localtime(&now);

    std::string hr = std::to_string(hr_offset + ltm->tm_hour);
    if(ltm->tm_hour < 10)
        hr = "0" + hr;
    std::string min = std::to_string(ltm->tm_min);
    if(ltm->tm_min < 10)
        min = "0" + min;
    std::string sec = std::to_string(ltm->tm_sec);
    if(ltm->tm_sec < 10)
        sec = "0" + sec;
    
    return hr + delim + min + delim + sec;
}