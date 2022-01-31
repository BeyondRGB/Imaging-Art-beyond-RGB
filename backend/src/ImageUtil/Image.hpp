#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <stdint.h>
#include <string>

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

    typedef unsigned short pixel;

    class image {
        public:
            image(std::string filename);
            ~image();

            void initBitmap(int width, int height, int channels);

            std::string filename();
            void setFilename(std::string filename);

            int width();
            int height();
            int channels();
            pixel* bitmap();
            uint32_t getIndex(int row, int col, int ch);
            void setPixel(int row, int col, int ch, btrgb::pixel value);
            btrgb::pixel getPixel(int row, int col, int ch);

            uint32_t getTotalByteSize();
            uint32_t getTotalPixelCount();
            uint32_t getRowByteSize();

            void recycle();

        private:
            std::string _filename;
            pixel* _bitmap = nullptr;
            int _width;
            int _height;
            int _channels;

            void checkInit();
    };

    class BitmapNotInitialized : public std::exception {
        private:
            std::string msg;
        public:
            BitmapNotInitialized(std::string msg) {
                this->msg = "The image \"" + msg + "\" has not been initialized.";
            }
            virtual char const * what() const noexcept { return  this->msg.c_str(); }
    };

    const int Red = 0;
    const int Green = 1;
    const int Blue = 2;
    const int R1 = 0;
    const int G1 = 1;
    const int B1 = 2;
    const int R2 = 3;
    const int G2 = 4;
    const int B2 = 5;
    const int Ch1 = 0;
    const int Ch2 = 1;
    const int Ch3 = 2;
    const int Ch4 = 3;
    const int Ch5 = 4;
    const int Ch6 = 5;
}

#endif
