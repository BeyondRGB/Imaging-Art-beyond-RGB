#ifndef BEYOND_RGB_BACKEND_BITDEPTHSCALERSTRATEGY_H
#define BEYOND_RGB_BACKEND_BITDEPTHSCALERSTRATEGY_H


#include "ImageUtil/Image.hpp"

class BitDepthScalerStrategy {
    public:
        virtual ~BitDepthScalerStrategy() {}
        virtual int get_bit_depth(btrgb::image* im) = 0;
        virtual void scale(btrgb::image* im);
        virtual void scale(btrgb::image* im, int bit_depth);
};

class BitDepthScalerStrategyError : public std::exception {};
    
class BitDepthScalerStrategy_InvalidBitDepth : public BitDepthScalerStrategyError {
    public:
    virtual char const * what() const noexcept { return "BitDepthScalerStrategy: Input bit depth must be between 8 and 16 inclusive."; }
};

class BitDepthScalerStrategy_InvalidScaler : public BitDepthScalerStrategyError {
    public:
    virtual char const * what() const noexcept { return "BitDepthScalerStrategy: Input scaler mues be between 2^0 and 2^8 inclusive."; }
};


#endif
