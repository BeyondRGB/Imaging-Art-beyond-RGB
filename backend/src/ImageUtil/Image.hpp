#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <stdint.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <filesystem>
namespace fs = std::filesystem;

/* Ways to loop:

    // Apply operation directly to mat.
    im->getMat() *= scaler;
   
    // Use Mat forEach loop (put variable from outside of for-loop in the capture list).
    float scaler = ...
    cv::Mat im32f = im->getMat();
    im32f.forEach<float[]>( [scaler](float (&pixel)[], const int* pos) -> void {
        pixel[R] *= scaler;
        pixel[G] *= scaler;
        pixel[B] *= scaler;
    });

    // Loop through every channel.
    float scaler = ...
    int channels = im->channels();
    cv::Mat im32f = im->getMat();
    im32f.forEach<float[]>( [channels](float (&pixel)[], const int* pos) -> void {
        for( int ch = 0; ch < channels; ch++) {
            pixel[ch] *= scaler;
        }
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

namespace btrgb {

    typedef std::unique_ptr<std::vector<uchar>> binary_ptr_t;
    typedef std::unique_ptr<std::string> base64_ptr_t;

    enum output_type {
        PNG,
        TIFF,
        JPEG
    };

    enum image_quality {
        FAST,
        FULL
    };


    class Image {
        public:
            Image(std::string filename);
            ~Image();

            void initImage(cv::Mat im);

            cv::Mat getMat();
            cv::Mat getMatCopyAs(int cv_type);

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

            void recycle();
            int _raw_bit_depth = 0;
            
            static bool is_tiff(std::string filename);

        private:
            std::string _name;
            float* _bitmap = nullptr;
            int _width = 0;
            int _height = 0;
            int _channels = 0;
            int _row_size = 0;
            int _col_size = 0;
            cv::Mat _opencv_mat;

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
