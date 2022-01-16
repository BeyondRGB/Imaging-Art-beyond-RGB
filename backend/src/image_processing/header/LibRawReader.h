#ifndef BEYOND_RGB_LIBRAWREADER_H
#define BEYOND_RGB_LIBRAWREADER_H

#include <math.h>
#include <memory>
#include <libraw.h>

#include "image_processing/header/RawReaderStrategy.h"
#include "ImageUtil/Image.hpp"

class LibRawReader : public RawReaderStrategy {

    public:
        LibRawReader();
        ~LibRawReader();
        void read(btrgb::image* im);

    private:
        class InternalRawProcessor : public LibRaw {
            public: void custom_process();
        };
        InternalRawProcessor rawReader;
        void configLibRawParams();

};

#endif