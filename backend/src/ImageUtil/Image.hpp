#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <stdint.h>
#include <string>
#include <opencv2/opencv.hpp>

/* Get array of channels for each pixel: (Use constants at end of Image.hpp)

    im->forEach([](float (&pixel)[], const int* pos) -> void {
        pixel[R] = 5;
        float blue = pixel[B];
        float blue1 = pixel[B1]; // B is the same as B1 so blue == blue1
        float green2 = pixel[G2]; // Only use R2, B2, & G2 for six channel images
    })


* Loop through every channel for each pixel: (put number of channels in capture list)

    int channels = im->channels();
    im->forEach( [channels](float (&pixel)[], const int* pos) -> void {
        for( int ch = 0; ch < channels; ch++) {
            pixel[ch] = <whatever value>;
            <whatever variable> = pixel[ch];
        }
    });

*/

namespace btrgb {

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

            std::string filename();
            void setFilename(std::string filename);

            void recycle();
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
