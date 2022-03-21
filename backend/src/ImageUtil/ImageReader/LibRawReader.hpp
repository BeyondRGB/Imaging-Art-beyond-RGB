#ifndef BTRGB_LIBRAWREADER_HPP
#define BTRGB_LIBRAWREADER_HPP

#include <math.h>
#include <memory>
#include <libraw.h>

#include "ImageReaderStrategy.hpp"

namespace btrgb {

class LibRawReader : public ImageReaderStrategy {

    public:
        enum libraw_type { PREVIEW, UNPROCESSED };
        LibRawReader(libraw_type method = UNPROCESSED);
        ~LibRawReader();

        void open(std::string filename) override;
        void recycle() override;

        void copyBitmapTo(void* buffer, uint32_t size) override;
        void copyBitmapTo(cv::Mat& im) override;

    private:
        LibRaw _reader;
        void _configLibRawParams();
        enum libraw_type _method = UNPROCESSED;

};

class LibRawFileTypeUnsupported : public ImageReaderStrategyError {
    public:
    virtual char const * what() const noexcept { return "RAW image is not supported by LibRaw."; }
};

}

#endif