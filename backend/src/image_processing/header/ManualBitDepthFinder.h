#include "image_processing/header/BitDepthScalerStrategy.h"

#define BIT_DEPTH_FINDER_PIXEL_COUNT_THRESHOLD 15

class ManualBitDepthFinder : public BitDepthScalerStrategy {
    public:
        ManualBitDepthFinder();
        ~ManualBitDepthFinder();
        int get_bit_depth(btrgb::image* im);
    private:
        void normal_bit_depth_loop(btrgb::image* im, int* bit_freq);
        void optimized_bit_depth_loop(btrgb::image* im, int* bit_freq);
};
