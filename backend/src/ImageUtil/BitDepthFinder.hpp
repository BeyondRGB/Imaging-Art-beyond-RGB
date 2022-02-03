#ifndef BTRGB_BIT_DEPTH_FINDER_HPP
#define BTRGB_BIT_DEPTH_FINDER_HPP

#include <stdint.h>


/* Test if built-in count leading zeros is available. */
#ifdef __has_builtin
    #if __has_builtin (__builtin_clz)
        #define BTRGB_MANUAL_BIT_DEPTH_USE_OPTIMIZED
    #endif
#endif


class BitDepthFinder {
    public:
        int get_bit_depth(uint16_t* im, int width, int height, int channels);
    private:
        const int PIXEL_COUNT_THRESHOLD = 15;
        inline int required_bits(uint16_t value);

};

#endif