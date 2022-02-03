#ifndef BEYOND_RGB_RAWREADERSTRATEGY_H
#define BEYOND_RGB_RAWREADERSTRATEGY_H

#include "ImageUtil/Image.hpp"

class RawReaderStrategy {
    public:
        virtual ~RawReaderStrategy() {}
        virtual void read(btrgb::Image* im) = 0;
};

class RawReaderStrategyError : public std::exception {};
    
class RawReaderStrategy_FailedToOpenFile : public RawReaderStrategyError {
    public:
    virtual char const * what() const noexcept { return "RawReaderStrategy Error: Unable to open file."; }
};

#endif