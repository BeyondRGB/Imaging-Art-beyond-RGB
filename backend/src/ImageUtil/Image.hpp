#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <stdint.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <unordered_map>
namespace fs = std::filesystem;

#include "ImageUtil/ColorProfiles.hpp"

/* Ways to loop:

    // Apply operation directly to mat.
    im->getMat() *= scaler;

    // Multi-threaded/parallel loop
    float scaler = ...
    cv::Mat im32f = im->getMat();
    // Three channel image
    im.forEach<cv::Vec3f>([scaler](cv::Vec3f& pixel, const int* pos) -> void {
        pixel[R] *= scaler;
        pixel[G] *= scaler;
        pixel[B] *= scaler;
    });
    // Six channel image
    im.forEach<cv::Vec<float,6>([](cv::Vec<float,6>& pixel, const int* pos) -> void {
        for( int ch = 0; ch < 6; ch++)
            pixel[ch] *= scaler;
        pixel[R1] *= scaler;
        pixel[G1] *= scaler;
        pixel[B1] *= scaler;
        pixel[R2] *= scaler;
        pixel[G2] *= scaler;
        pixel[B2] *= scaler;
    });

    // Direct looping.
    int row, col, ch;
    int width = im->width(), height = im->height(), channels = im->channels();
    float pixel_value;
    float* pixel_ptr;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            
            // Every channel
            for (ch = 0; ch < channels; ch++) {
                pixel_value = im->getPixel(row, col, ch);
                im->setPixel(currRow, col, ch, 4.645);
            }

            // Access specific channels
            pixel_ptr = im->getPixelPointer(row, col);
            pixel_ptr[R] = 3;
            pixel_value = pixel_ptr[G1];
        }
    }
*/

#define BTRGB_M_OPT "M_opt"
#define BTRGB_OFFSET_OPT "Offset_opt"
#define BTRGB_M_REFL_OPT "M_refl_opt"

namespace btrgb {

    typedef std::unique_ptr<std::vector<uchar>> binary_ptr_t;
    typedef std::unique_ptr<std::string> base64_ptr_t;

    enum output_type {
        PNG,
        TIFF,
        JPEG
    };

    enum image_quality {
        FAST, FULL
    };

    class Image {
        public:
            Image(std::string filename);
            ~Image();

            void initImage(cv::Mat im);

            cv::Mat getMat();

            int width();
            int height();
            int channels();
            float* bitmap();

            uint32_t getIndex(int row, int col, int ch);
            void setPixel(int row, int col, int ch, float value);
            float getPixel(int row, int col, int ch);
            float* getPixelPointer(int row, int col);

            std::string getName();
            void setName(std::string name);

            binary_ptr_t getEncodedPNG(enum image_quality quality);

            void setColorProfile(ColorSpace color_profile);
            ColorSpace getColorProfile();
            
            void setConversionMatrix(std::string key, cv::Mat m);
            cv::Mat getConversionMatrix(std::string key);
            class ConversionsItr {
                public:
                    typedef std::unordered_map<std::string, cv::Mat>::iterator conv_itr;
                    ConversionsItr(conv_itr b, conv_itr e) : _begin(b), _end(e) {}
                    conv_itr begin() noexcept {return _begin;};
                    conv_itr end() noexcept {return _end;};
                private:
                    conv_itr _begin;
                    conv_itr _end;
            };
            ConversionsItr getConversionsIterator() {
                return ConversionsItr(_conversions.begin(), _conversions.end());
            }


            void recycle();
            std::shared_ptr<int> _raw_bit_depth;
            
            /* ====== static ======= */
            static bool is_tiff(std::string filename);
            static cv::Mat copyMatConvertDepth(cv::Mat input, int cv_depth);
            

        private:
            std::string _name;
            float* _bitmap = nullptr;
            int _width = 0;
            int _height = 0;
            int _channels = 0;
            int _row_size = 0;
            int _col_size = 0;
            cv::Mat _opencv_mat;
            ColorSpace _color_profile = none;
            std::unordered_map<std::string, cv::Mat> _conversions;

            void _checkInit();
    };

    class ImageError : public std::exception {};

    class ImageNotInitialized : public ImageError {
        private:
            std::string msg;
        public:
            ImageNotInitialized(std::string msg) {
                this->msg = "The Image \"" + msg + "\" has not been initialized.";
            }
            virtual char const * what() const noexcept { return  this->msg.c_str(); }
    };

    class FailedToEncode : public ImageError {
        public:
            virtual char const * what() const noexcept { return "[Image::to<Binary|Base64>OfType] OpenCV failed to encode image."; }
    };

    class UnsupportedChannels : public ImageError {
        public:
            virtual char const * what() const noexcept { return "Image::initBitmap(): The number of channels must be between 1 and 10 inclusive."; }
    };
    
}

const int R = 0;
const int G = 1;
const int B = 2;

const int R1 = 0;
const int G1 = 1;
const int B1 = 2;
const int R2 = 3;
const int G2 = 4;
const int B2 = 5;

#endif
