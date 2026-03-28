#ifndef FLATFIELD_STATIC_H
#define FLATFIELD_STATIC_H

#include <image_util/Image.hpp>

namespace btrgb {
namespace flatfield {

/**
 * Updates the pixels based on the w calculation for both given images
 * @param h: height of images
 * @param wid: width of images
 * @param c: channel count
 * @param a1: art1 image
 * @param a2: art2 image
 * @param wh1: white1 image
 * @param wh2: white2 image
 * @param d1: dark1 image
 * @param d2 : dark2 image
 */
void pixelOperation(float w, int row, int col, int img_hei, 
                                   int img_wid, int chunk_hei, int chunk_wid, int c, 
                                   btrgb::Image *a, btrgb::Image *wh, 
                                   btrgb::Image *d, btrgb::Image *ac);

} // namespace flatfield
} // namespace btrgb

#endif // FLATFIELD_STATIC_H