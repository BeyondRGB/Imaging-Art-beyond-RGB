#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <stdint.h>
#include <string>
#include <opencv2/opencv.hpp>

/*

im->forEach([](float (&pixel)[], const int* position) -> void {
    pixel[R] = 5;
    float blue = pixel[B];
    float blue2 = pixel[G2];
})

*/

namespace btrgb {

    class Image {
        public:
            Image(std::string filename);
            ~Image();

            void initImage(int width, int height, int channels);

            cv::Mat getMat();

            int width();
            int height();
            int channels();
            float* bitmap();

            uint32_t getIndex(int row, int col, int ch);
            void Image::setPixel(int row, int col, int ch, float value);
            float Image::getPixel(int row, int col, int ch);

            float* Image::getPixelPointer(int row, int col);
            template<typename T> inline void forEach(const T& operation);

            std::string filename();
            void setFilename(std::string filename);

            void recycle();
            cv::Mat _temp;
            int _raw_bit_depth = 0;

        private:
            std::string _filename;
            float* _bitmap = nullptr;
            int _width = 0;
            int _height = 0;
            int _channels = 0;
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
