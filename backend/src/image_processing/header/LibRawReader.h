#ifndef BEYOND_RGB_LIBRAWREADER_H
#define BEYOND_RGB_LIBRAWREADER_H

#include <stdint.h>
#include <math.h>
#include <memory>
#include <libraw.h>
#include <opencv2/opencv.hpp>

#include "image_processing/header/RawReaderStrategy.h"
#include "ImageUtil/Image.hpp"
#include "ImageUtil/BitDepthFinder.hpp"

class LibRawReader : public RawReaderStrategy {

    public:
        const static bool LOAD_HALF_SIZE = true;
        LibRawReader(bool use_half_size = false);
        ~LibRawReader();

        void read(btrgb::Image* im, bool record_bit_depth = false);

    private:
        class InternalRawProcessor : public LibRaw {
            public: void custom_process();
        };
        InternalRawProcessor rawReader;
        bool use_half_size = false;
        void configLibRawParams();

};

#endif