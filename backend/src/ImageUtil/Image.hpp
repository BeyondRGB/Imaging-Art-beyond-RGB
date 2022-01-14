#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <string>

/* How to iterate over the bitmap:
 * 
 * ch = 0: red channel
 * ch = 1: green channel
 * ch = 2: blue channel
 *
 * 
 * Iterate over every channel value for each pixel:
 * 
 *    int ch, x, y, i, ix, iy;
 *    for( y = 0; y < height; y++) {
 *        iy = y * width * channels;
 *        for( x = 0; x < width; x++) {
 *            ix = x * channels;
 *            for( ch = 0; ch < channels; ch++) {
 *                i = iy + ix + ch;
 *                // i is the index for the bitmap
 *            }
 *        }
 *    }
 * 
 * 
 * Iterate over exery pixel in each channel:
 * 
 *    int ch, x, y, i, ix, iy;
 *    for( ch = 0; ch < channels; ch++) {
 *        for( y = 0; y < height; y++) {
 *            iy = y * width * channels;
 *            for( x = 0; x < width; x++) {
 *                ix = x * channels;
 *                i = iy + ix + ch;
 *                // i is the index for the bitmap
 *            }
 *        }
 *    }
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
            int width();
            int height();
            int channels();
            pixel* bitmap();

            void recycle();

        private:
            std::string _filename;
            pixel* _bitmap;
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
}

#endif