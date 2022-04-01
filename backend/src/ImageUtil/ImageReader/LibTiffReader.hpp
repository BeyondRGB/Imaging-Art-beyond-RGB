#ifndef BTRGB_LIBTIFFREADER_HPP
#define BTRGB_LIBTIFFREADER_HPP

extern "C" {
    #include <tiffio.h>
}

#include "ImageReaderStrategy.hpp"

namespace btrgb {

class LibTiffReader : public ImageReaderStrategy {

    public:
        LibTiffReader();
        ~LibTiffReader();

        void open(std::string filename) override;
        void recycle() override;

        void copyBitmapTo(void* buffer, uint32_t size) override;
        void copyBitmapTo(cv::Mat& im) override;

    private:
        //LibRaw _reader;

};

}

#endif