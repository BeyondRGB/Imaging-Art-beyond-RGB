#ifndef FLATFIELD_STATIC_H
#define FLATFIELD_STATIC_H

#include <image_util/Image.hpp>

namespace btrgb {
namespace flatfield {

/**
 * Updates the pixels based on the w calculation for both given images
 * @param w: white patch
 * @param row row to begin the operation on
 * @param col col to begin the operation on
 * @param img_hei: height of images
 * @param img_wid: width of images
 * @param chunk_hei: height of the chunk we wish to operate on
 * @param chunk_wid: width of the chunk we wish to operate on
 * @param c: channel count
 * @param a: art1 image
 * @param wh: white1 image
 * @param d : dark1 image
 * @param ac : art copy image
 */
void pixelOperation(float w, int row, int col, int img_hei, int img_wid,
                    int chunk_hei, int chunk_wid, int c, btrgb::Image *a,
                    btrgb::Image *wh, btrgb::Image *d, btrgb::Image *ac);

} // namespace flatfield
} // namespace btrgb

#endif // FLATFIELD_STATIC_H