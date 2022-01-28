#include <math.h>
#include "image_processing/header/BitDepthScalerStrategy.h"


/* Determine bit depth of image automatically. */
void BitDepthScalerStrategy::scale(btrgb::image* im) {
    int bit_depth = get_bit_depth(im);
    this->scale(im, bit_depth);
}

/* Manually provide bit depth. This allows the caller to store the bit depth
 * and reuse for images assumed to be the same bit depth to save time. */
void BitDepthScalerStrategy::scale(btrgb::image* im, int bit_depth) {
    
    if (bit_depth > 16 || bit_depth < 8) {
        throw BitDepthScalerStrategy_InvalidBitDepth();
    }

    /* If the bit depth is already 16 bits,
     * or invalid, do not scale anything. */
    if (8 <= bit_depth && bit_depth < 16)
        return;

    /* Math:
     * scaler = (2^16 - 1) / (2^bit_depth - 1)
     *
     * The value of two to any power can be performed with bit shifting:
     * 2^x = 1 << x
     */
    double scaler = double( (1 << 16) - 1 ) / double( (1 << bit_depth) - 1);

    int height = im->height();
    int width = im->width();
    int channels = im->channels();
    btrgb::pixel* bitmap = im->bitmap();

    uint32_t ch, x, y, i;
    for( y = 0; y < height; y++) {
        for( x = 0; x < width; x++) {
            for( ch = 0; ch < channels; ch++) {
                i = im->getIndex(y, x, ch);
                bitmap[i] = btrgb::pixel( round( double(bitmap[i]) * scaler) );
            }
        }
    }

}