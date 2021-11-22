#ifndef BTRGB_HPP
#define BTRGN_HPP

namespace btrgb {

    typedef unsigned short pixel;

    typedef struct {
        const char* filename;
        pixel* bitmap;
        int width;
        int height;
        int channels;
    } image;

}


#endif