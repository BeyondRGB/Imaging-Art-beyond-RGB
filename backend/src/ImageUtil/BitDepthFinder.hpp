#ifndef BTRGB_BIT_DEPTH_FINDER_HPP
#define BTRGB_BIT_DEPTH_FINDER_HPP

#include <stdint.h>


class BitDepthFinder {
    public:
        int get_bit_depth(uint16_t* im, int width, int height, int channels);
    private:
        const int PIXEL_COUNT_THRESHOLD = 15;
        inline int required_bits(uint16_t value);

};

#endif