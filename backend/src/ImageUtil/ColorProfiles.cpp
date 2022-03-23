#include "ColorProfiles.hpp"

namespace btrgb {

cv::Mat ColorProfiles::convert(cv::Mat im, ColorSpace from, ColorSpace to) {
    if( to == none )
        throw std::logic_error("[ColorProfiles] Color space conversion to \"none\" not implemented.");
    if( from == none )
        return im;
    
    /* Change the shape to:
     *      #channels=1, #columns=#channels, #rows=#pixels
     * then transpose to: 
     *      #channels=1, #columns=#pixels, #rows=#channels */
    int pixel_count = im.rows * im.cols;
    cv::Mat pixels_in = im.reshape(1, pixel_count).t();

    /* Get conversion matricies. */
    cv::Mat input_to_xyz = XYZ_to_RGB[from].inv();
    cv::Mat xyz_to_output= XYZ_to_RGB[to];

    /* Convert input color space to XYZ. */
    cv::Mat pixels_xyz;
    pixels_xyz = pixels_in * input_to_xyz;
    pixels_in.release();

    /* Convert XYZ to target color space. */
    cv::Mat pixels_out;
    pixels_out = pixels_xyz * xyz_to_output;
    pixels_xyz.release();

    /* Transpose the image back, and reshape to original dimensions. */
    cv::Mat result = pixels_out.t();
    return result.reshape(im.channels(), im.rows);

}

};