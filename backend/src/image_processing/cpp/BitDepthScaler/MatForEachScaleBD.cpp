#include "image_processing/header/MatForEachScaleBD.h"


/* Manually provide bit depth. This allows the caller to store the bit depth
 * and reuse for images assumed to be the same bit depth to save time. */
void MatForEachScaleBD::scale(btrgb::Image* im) {

    int raw_bit_depth = im->_raw_bit_depth;
    
    if (raw_bit_depth > 16 || raw_bit_depth < 8) {
        throw BitDepthScalerStrategy_InvalidBitDepth();
    }

    /* If the bit depth is already 16 bits, do not scale anything. */
    if (raw_bit_depth == 16)
        return;

    /* Math:
     * scaler = (2^16 - 1) / (2^bit_depth - 1)
     *
     * The value of two to any power can be performed with bit shifting:
     * 2^x = 1 << x
     */
    float scaler = float( (1 << 16) - 1 ) / float( (1 << raw_bit_depth) - 1);

    im->getMat() *= scaler;
    /*
    cv::Mat im32f = im->getMat();
    im32f.forEach<float[]>( [scaler](float (&pixel)[], const int* pos) -> void {
        pixel[R] *= scaler;
        pixel[G] *= scaler;
        pixel[B] *= scaler;
    });
    */

}