#ifndef BEYOND_RGB_BACKEND_BITDEPTHSCALERSTRATEGY_H
#define BEYOND_RGB_BACKEND_BITDEPTHSCALERSTRATEGY_H

#include "ImageUtil/Image.hpp"

class BitDepthScalerStrategy {
    public:
        virtual ~BitDepthScalerStrategy() {}
        virtual void scale(btrgb::Image* im) = 0;
};

class BitDepthScalerStrategyError : public std::exception {};
    
class BitDepthScalerStrategy_InvalidBitDepth : public BitDepthScalerStrategyError {
    public:
    virtual char const * what() const noexcept { return "BitDepthScalerStrategy: Input bit depth must be between 8 and 16 inclusive."; }
};


#endif
