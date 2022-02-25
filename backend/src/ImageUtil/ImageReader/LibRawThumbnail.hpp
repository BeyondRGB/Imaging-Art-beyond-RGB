#ifndef BTRGB_LIBRAWTHUMBNAIL_HPP
#define BTRGB_LIBRAWTHUMBNAIL_HPP

#include <math.h>
#include <memory>
#include <libraw.h>

#include "ImageReaderStrategy.hpp"

namespace btrgb {

class LibRawThumbnail : public ImageReaderStrategy {

    public:
        LibRawThumbnail();
        ~LibRawThumbnail();

        void open(std::string filename);
        void recycle();
        bool is_encoded();
        unsigned int length();

        void copyBitmapTo(void* buffer, uint32_t size);
        void copyBitmapTo(cv::Mat& im);

    private:
        LibRaw _reader;
        bool _is_encoded;
        bool _length;
        void* _data;

};

}

#endif