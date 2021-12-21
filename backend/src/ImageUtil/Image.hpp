#ifndef BTRGB_IMAGE_H
#define BTRGB_ARTOBJ_H

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

            void recylce();

        private:
            std::string _filename;
            pixel* _bitmap;
            int _width;
            int _height;
            int _channels;
    };
}

#endif