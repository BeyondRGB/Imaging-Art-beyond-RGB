#ifndef BTRGB_LIBRAWREADER_HPP
#define BTRGB_LIBRAWREADER_HPP


#include "ImageReaderStrategy.hpp"

namespace btrgb {

class TiffReaderOpenCV : public ImageReaderStrategy {

    public:
        TiffReaderOpenCV();
        ~TiffReaderOpenCV();

        void open(std::string filename);
        void recycle();

        void copyBitmapTo(void* buffer, uint32_t size);
        void copyBitmapTo(cv::Mat& im);

    private:
        cv::Mat _im;

};

}

#endif