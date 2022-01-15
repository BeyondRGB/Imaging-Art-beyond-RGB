#ifndef BEYOND_RGB_LIBRAWFILEREADER_H
#define BEYOND_RGB_LIBRAWFILEREADER_H

#include <math.h>
#include <memory>
#include <libraw.h>

#include "image_processing/header/RawFileReader.h"
#include "ImageUtil/Image.hpp"

class LibRawFileReader : public RawFileReader {

    public:
        LibRawFileReader();
        ~LibRawFileReader();
        void read(btrgb::image* im);

    private:
        class InternalRawProcessor : public LibRaw {
            public: void custom_process();
        };
        InternalRawProcessor rawReader;
        void configLibRawParams();

};

#endif