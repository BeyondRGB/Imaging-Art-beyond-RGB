#include <math.h>
#include "image_processing/header/BitDepthScalerStrategy.h"


/* Determine bit depth of image automatically. */
void BitDepthScalerStrategy::scale(btrgb::Image* im) {
    int bit_depth = get_bit_depth(im);
    this->scale(im, bit_depth);
}

/* Manually provide bit depth. This allows the caller to store the bit depth
 * and reuse for images assumed to be the same bit depth to save time. */
void BitDepthScalerStrategy::scale(btrgb::Image* im, int bit_depth) {
    
    if (bit_depth > 16 || bit_depth < 8) {
        throw BitDepthScalerStrategy_InvalidBitDepth();
    }

    /* If the bit depth is already 16 bits, do not scale anything. */
    if (bit_depth == 16)
        return;

    /* Math:
     * scaler = (2^16 - 1) / (2^bit_depth - 1)
     *
     * The value of two to any power can be performed with bit shifting:
     * 2^x = 1 << x
     */
    float scaler = float( (1 << 16) - 1 ) / float( (1 << bit_depth) - 1);

    int channels = im->_temp.channels();

    im->_temp.forEach<float[]>([channels, scaler](float (&pixel)[], const int* pos) -> void {
        for( int ch = 0; ch < channels; ch++) {
            pixel[ch] = uint16_t( round( float(pixel[ch]) * scaler) );
        }
    });


}