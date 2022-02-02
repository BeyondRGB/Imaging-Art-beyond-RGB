#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <stdint.h>
#include <string>
#include <opencv2/opencv.hpp>

/* How to iterate over the bitmap:
 * 
 * ch = 0: red channel
 * ch = 1: green channel
 * ch = 2: blue channel
 *
 * 
 * Iterate over every channel value for each pixel:

    int height = im->height();
    int width = im->width();
    int channels = im->channels();
    btrgb::pixel* bitmap = im->bitmap();

    uint32_t ch, x, y, i;
    for( y = 0; y < height; y++) {
        for( x = 0; x < width; x++) {
            for( ch = 0; ch < channels; ch++) {
                i = im->getIndex(y, x, ch);
                //<some variable> = bitmap[i]; //getting
                //bitmap[i] = <some number>; //setting
            }
        }
    }

 * 
 * 
 * Alternative: iterate over exery pixel in each channel:
 * 
    int height = im->height();
    int width = im->width();
    int channels = im->channels();
    btrgb::pixel* bitmap = im->bitmap();

    uint32_t ch, x, y, i;
    for( ch = 0; ch < channels; ch++) {
        for( y = 0; y < height; y++) {
            for( x = 0; x < width; x++) {
                    i = im->getIndex(y, x, ch);
                    //<some variable> = bitmap[i]; //getting
                    //bitmap[i] = <some number>; //setting
                }
            }
        }
    }
 * 
 * Format of bitmaps:
 * 
 * Three channels:
 * |               Row 1               |                  Row 2                    | number of rows = height
 * |    Col 1  |   Col 2   |  Col 3    |    Col 1    |    Col 2     |    Col 3     | number of columns = width
 * | R | G | B | R | G | B | R | G | B | R | G  | B  | R  | G  | B  | R  | G  | B  | channel
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | bitmap array index
 *
 * Six channels:
 * |                                          Row 1                                          | number of rows = height
 * |            Col 1            |            Col 2            |            Col 3            | number of columns = width
 * | Ch1| Ch2| Ch3| Ch4| Ch5| Ch6| Ch1| Ch2| Ch3| Ch4| Ch5| Ch6| Ch1| Ch2| Ch3| Ch4| Ch5| Ch6| channel
 * |  0 | 1  |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | bitmap array index
 * 
 */

namespace btrgb {

    class Image {
        public:
            Image(std::string filename);
            ~Image();

            void initBitmap(int width, int height, int channels);

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

    const int R = 0;
    const int G = 1;
    const int B = 2;

    const int R1 = 0;
    const int G1 = 1;
    const int B1 = 2;
    const int R2 = 3;
    const int G2 = 4;
    const int B2 = 5;

}

#endif
