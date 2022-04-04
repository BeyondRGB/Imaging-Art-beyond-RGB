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

        cv::Mat getCrop(uint32_t left, uint32_t right, uint32_t width, uint32_t height);

    private:
        struct tiff* _tiff;

};

}

#endif