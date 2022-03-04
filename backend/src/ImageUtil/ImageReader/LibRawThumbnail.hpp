#ifndef BTRGB_LIBRAWTHUMBNAIL_HPP
#define BTRGB_LIBRAWTHUMBNAIL_HPP

#include <cstring>

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
        size_t length();


        void copyBitmapTo(std::vector<uchar>& buffer);
        void copyBitmapTo(void* buffer, uint32_t size);
        void copyBitmapTo(cv::Mat& im);

    private:
        LibRaw _reader;
        bool _is_encoded;
        size_t _length;
        void* _data;

};

}

#endif