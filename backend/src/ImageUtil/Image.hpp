#ifndef BTRGB_IMAGE_H
#define BTRGB_IMAGE_H

#include <string>

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