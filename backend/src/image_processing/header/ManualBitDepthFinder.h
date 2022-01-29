#include "image_processing/header/BitDepthScalerStrategy.h"

#define BIT_DEPTH_FINDER_PIXEL_COUNT_THRESHOLD 15

/* Test if built-in count leading zeros is available. */
#ifdef __has_builtin
    #if __has_builtin (__builtin_clz)
        #define BTRGB_MANUAL_BIT_DEPTH_USE_OPTIMIZED
    #endif
#endif


class ManualBitDepthFinder : public BitDepthScalerStrategy {
    public:
        ManualBitDepthFinder();
        ~ManualBitDepthFinder();
        int get_bit_depth(btrgb::image* im);
    private:
        inline int required_bits(btrgb::pixel value);
};
