#include "image_processing/header/ManualBitDepthFinder.h"


ManualBitDepthFinder::ManualBitDepthFinder() {}
ManualBitDepthFinder::~ManualBitDepthFinder() {}

/* This function accounts for the small chance that, for example,
 * a 14 bit image may have one or two pixels take up 15 bits. 
 * Just in case if there were such an error, the number of pixels
 * for the maximum bit depth must be above a given threshold. 
 */
int ManualBitDepthFinder::get_bit_depth(btrgb::image* im) {

    /* Histogram: group pixels by the number of 
     * bits needed to represent their values. */
    int histogram[17] = {0};
    int* bit_freq = histogram;
    int b;

     /* Values used to loop through every
     * pixel in the image. */
    int height = im->height();
    int width = im->width();
    int channels = im->channels();
    btrgb::pixel* bitmap = im->bitmap();

    /* For every pixel in the image... */
    uint32_t ch, x, y, i;
    for( y = 0; y < height; y++) {
        for( x = 0; x < width; x++) {
            for( ch = 0; ch < channels; ch++) {
                i = im->getIndex(y, x, ch);
                b = this->required_bits(bitmap[i]);
                bit_freq[b]++;
            }
        }
    }

    /* Check the histogram to find the higest required-bits
     * value that has a pixel count above the threshold. */
    for( b = 16; b >= 8; b--) {
        if( bit_freq[b] > BIT_DEPTH_FINDER_PIXEL_COUNT_THRESHOLD) {
            return b;
        }
    }

     /* Bit depth not within range. */
    return -1;
}


int ManualBitDepthFinder::required_bits(btrgb::pixel value) {

    int bits;

    #ifdef BTRGB_MANUAL_BIT_DEPTH_USE_OPTIMIZED

        bits = __builtin_clz(value);

        /* We want the non-leading zero bits:
        * 32 - <leading zeros>
        *
        * Even though our number is 16 bits, the above still checks 
        * out because leading bits for a 16 bit number should be 16 
        * less than the leading zeros for a 32 bit number:
        * <lzs 16 bit> = <lzs 32 bit> - 16
        *
        * Then, the the non-leading zero bits for that:
        * 16 - <lzs 16 bit>
        * = 16 - (<lzs 32 bit> - 16)
        * = 32 - <lzs 32 bit>
        */
        return 32 - bits;

    #else
        /* Determine the number of bits required
        * to represent the given pixel. Ignore
        * pixel if less than 8 bits. Increment
        * the frequency for that specific number
        * of bits.
        * 
        * Example:
        *   Given a pixel,
        *     pixel = 12517 (14 bits: 0011000011100101)
        * 
        *   If you shift it to the right by 15, 
        *   (which is 16 minus 1) the value is 0.
        * 
        *   If you shift it to the right by 14, 
        *   (which is 15 minus 1) the value is 0.
        * 
        *   If you shift it to the right by 13, 
        *   (which is 14 minus 1) the value is 1.
        * 
        *   Since the highest number of shifts 
        *   to the right that yields 1 is 13,
        *   the number of bits needed for that
        *   pixel is 13 + 1 which is 14 bits. 
        */
        for( bits = 16; bits >= 8; bits--) {
            if(value >> (bits - 1)) {
                return bits;
            }
        }
        /* Don't bother if the number is below 8 bits.
         * to save time. */
        return 8;
    #endif
            
}
