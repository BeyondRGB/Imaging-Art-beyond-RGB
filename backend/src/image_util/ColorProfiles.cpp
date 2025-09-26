#include <image_util/ColorProfiles.hpp>

namespace btrgb {


void ColorProfiles::convert(cv::Mat im,
        void* from_profile_data, uint32_t from_size,
        void* to_profile_data, uint32_t to_size) {

    cmsUInt32Number type;
    switch(im.depth()) {
        case CV_16U: type = TYPE_RGB_16; break;
        case CV_8U: type = TYPE_RGB_8; break;
        case CV_32F: type = TYPE_RGB_FLT; break;
        default: 
            throw std::runtime_error("[ColorProfiles::convert] Mat type not supported.");
    }

    cmsHPROFILE from_profile, to_profile;
    from_profile = cmsOpenProfileFromMem(from_profile_data, from_size);
    to_profile = cmsOpenProfileFromMem(to_profile_data, to_size);

    cmsHTRANSFORM colorspace_transform;
    colorspace_transform = cmsCreateTransform(from_profile, type, to_profile, type, INTENT_PERCEPTUAL, 0);
    cmsDoTransform(colorspace_transform, im.data, im.data, im.rows * im.cols);
    cmsDeleteTransform(colorspace_transform);

    cmsCloseProfile(from_profile);
    cmsCloseProfile(to_profile);
}

void ColorProfiles::convert(cv::Mat im, ColorSpace from, ColorSpace to) {

    if(from == to)
        return;

    else if(im.channels() != 3)
        throw std::runtime_error("[ColorProfiles::convert] Only supports three-channel images.");

    else if(from == ColorSpace::none)
        ColorProfiles::apply_gamma(im, ColorSpace::sRGB);
    
    else {
        ColorProfiles::linearize(im, from);
        ColorProfiles::convert_to_xyz(im, from);
        ColorProfiles::convert_to_color(im, to);
        ColorProfiles::apply_gamma(im, to);
    }

}


void ColorProfiles::multiply_conversion_matrix(cv::Mat im, cv::Mat m) {
    /* Change the input image shape to:
     * #channels=1, #columns=#channels, #rows=#pixels
     * RGB
     * RGB
     * RGB
     * RGB 
     * ...
     * [pixel_vectors] is the same underlying data as [im], 
     * just with a different matrix header.
     */
    int pixel_count = im.rows * im.cols;
    cv::Mat pixel_vectors = im.reshape(1, pixel_count);

    /* Multiply matricies:
     *
     * Using:
     * M: 3x3 conversion matrix
     * I: 3xn image matrix
     * It: nx3 image matrix (transpose of I)
     * 
     * OpenCV can convert from a normal image to It format 
     * in O(1) time. The result we want is (MxI)t and that
     * would require taking the transpose of our input format.
     * Since we already have It we can instead use the 
     * property: (MxI)t = It x Mt. M is only 3x3 and is
     * easier to transpose. This also does not require an
     * additional transpose to get back to the It format.*/
    pixel_vectors *= m.t();

}


void ColorProfiles::convert_to_xyz(cv::Mat im, ColorSpace from) {
    cv::Mat m;

    switch(from) {
    case ColorSpace::ProPhoto:
        m = (cv::Mat_<float>(3,3) <<
            0.7976749, 0.1351917, 0.0313534,
            0.2880402, 0.7118741, 0.0000857,
            0.0000000, 0.0000000, 0.8252100
        ); break;

    case ColorSpace::sRGB:
        m = (cv::Mat_<float>(3,3) <<
            0.4360747, 0.3850649, 0.1430804,
            0.2225045, 0.7168786, 0.0606169,
            0.0139322, 0.0971045, 0.7141733
        ); break;

    case ColorSpace::Adobe_RGB_1998:
        m = (cv::Mat_<float>(3,3) <<
            0.6097559, 0.2052401, 0.1492240,
            0.3111243, 0.6256560, 0.0632197,
            0.0194811, 0.0608902, 0.7448386
        ); break;

    case ColorSpace::Wide_Gamut_RGB:
        m = (cv::Mat_<float>(3,3) <<
            0.7161046, 0.1009296, 0.1471858,
            0.2581873, 0.7249378, 0.0168748,
            0.0000000, 0.0517813, 0.7734287
        ); break;

    default:
        throw std::runtime_error("[ColorProfiles::convert_to_xyz] Not implemented. ");
    }

    ColorProfiles::multiply_conversion_matrix(im, m);
}


void ColorProfiles::convert_to_color(cv::Mat im, ColorSpace to) {
    cv::Mat m;

    switch(to) {
    case ColorSpace::ProPhoto:
        m = (cv::Mat_<float>(3,3) <<
             1.3459433, -0.2556075, -0.0511118,
            -0.5445989,  1.5081673,  0.0205351,
             0.0000000,  0.0000000,  1.2118128
        ); break;

    case ColorSpace::Adobe_RGB_1998:
        m = (cv::Mat_<float>(3,3) <<
             1.9624274, -0.6105343, -0.3413404,
            -0.9787684,  1.9161415,  0.0334540,
             0.0286869, -0.1406752,  1.3487655
        ); break;

    case ColorSpace::sRGB:
        m = (cv::Mat_<float>(3,3) <<
             3.1338561, -1.6168667, -0.4906146,
            -0.9787684,  1.9161415,  0.0334540,
             0.0719453, -0.2289914 , 1.4052427
        ); break;

    case ColorSpace::Wide_Gamut_RGB:
        m = (cv::Mat_<float>(3,3) <<
             1.4628067, -0.1840623, -0.2743606,
            -0.5217933,  1.4472381,  0.0677227,
             0.0349342, -0.0968930,  1.2884099
        ); break;

    default:
        throw std::runtime_error("[ColorProfiles::convert_to_color] Not implemented. ");
    }

    ColorProfiles::multiply_conversion_matrix(im, m);

    #define BTRGB_CLIP_PIXEL(x) (x < 0 ? 0 : (x > 1 ? 1 : x))
    im.forEach<cv::Vec3f>([](cv::Vec3f& pixel, const int* pos) -> void {
        pixel[R] = BTRGB_CLIP_PIXEL(pixel[R]);
        pixel[G] = BTRGB_CLIP_PIXEL(pixel[G]);
        pixel[B] = BTRGB_CLIP_PIXEL(pixel[B]);
    });
    #undef BTRGB_CLIP_PIXEL
}


void ColorProfiles::linearize(cv::Mat im, ColorSpace from) {
    #define BTRGB_PROPHOTO_INVERSE_GAMMA(x) (x >= 0.03125 ? pow(x, 1.8) : x / 16)
    #define BTRGB_sRGB_INVERSE_GAMMA(x) (x > 0.040449936 ? pow((x + 0.055) / 1.055, 2.4): x / 12.92)

    switch(from) {

        case ColorSpace::ProPhoto:
            im.forEach<cv::Vec3f>([](cv::Vec3f& pixel, const int* pos) -> void {
                pixel[R] = BTRGB_PROPHOTO_INVERSE_GAMMA(pixel[R]);
                pixel[G] = BTRGB_PROPHOTO_INVERSE_GAMMA(pixel[G]);
                pixel[B] = BTRGB_PROPHOTO_INVERSE_GAMMA(pixel[B]);
            }); break;

        case ColorSpace::sRGB:
            im.forEach<cv::Vec3f>([](cv::Vec3f& pixel, const int* pos) -> void {
                pixel[R] = BTRGB_sRGB_INVERSE_GAMMA(pixel[R]);
                pixel[G] = BTRGB_sRGB_INVERSE_GAMMA(pixel[G]);
                pixel[B] = BTRGB_sRGB_INVERSE_GAMMA(pixel[B]);
            }); break;

        default:
            throw std::runtime_error("[ColorProfiles::inverse_gamma] Not implemented. ");
    }
        
    #undef BTRGB_PROPHOTO_INVERSE_GAMMA
    #undef BTRGB_sRGB_INVERSE_GAMMA
}


void ColorProfiles::apply_gamma(cv::Mat im, ColorSpace to) {
    #define BTRGB_PROPHOTO_GAMMA(x) (x >= 0.001953125 ? pow(x, 1/1.8) : x * 16)
    #define BTRGB_sRGB_GAMMA(x) (x > 0.0031308 ? 1.055 * pow(x, 1/2.4) - 0.055 : x * 12.92)

    int channels = im.channels();
    switch(to) {

        case ColorSpace::ProPhoto:
            im.forEach<cv::Vec3f>([](cv::Vec3f& pixel, const int* pos) -> void {
                pixel[R] = BTRGB_PROPHOTO_GAMMA(pixel[R]);
                pixel[G] = BTRGB_PROPHOTO_GAMMA(pixel[G]);
                pixel[B] = BTRGB_PROPHOTO_GAMMA(pixel[B]);
            }); break;

        case ColorSpace::sRGB:
            im.forEach<cv::Vec3f>([](cv::Vec3f& pixel, const int* pos) -> void {
                pixel[R] = BTRGB_sRGB_GAMMA(pixel[R]);
                pixel[G] = BTRGB_sRGB_GAMMA(pixel[G]);
                pixel[B] = BTRGB_sRGB_GAMMA(pixel[B]);
            }); break;

        default:
            throw std::runtime_error("[ColorProfiles::apply_gamma] Not implemented. ");
    }
    
    #undef BTRGB_PROPHOTO_GAMMA
    #undef BTRGB_sRGB_GAMMA
}



};