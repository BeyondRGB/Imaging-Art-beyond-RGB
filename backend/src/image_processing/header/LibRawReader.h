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
        LibRawReader(bool use_half_size);
        const static bool LOAD_HALF_SIZE = true;
        ~LibRawReader();

        void read(btrgb::image* im);

    private:
        class InternalRawProcessor : public LibRaw {
            public: void custom_process();
        };
        InternalRawProcessor rawReader;
        bool use_half_size = false;
        void configLibRawParams();

};

#endif