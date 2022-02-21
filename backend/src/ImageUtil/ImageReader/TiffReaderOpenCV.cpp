#include <string.h>
#include "TiffReaderOpenCV.hpp"

namespace btrgb {

TiffReaderOpenCV::TiffReaderOpenCV() {}
TiffReaderOpenCV::~TiffReaderOpenCV() {}

void TiffReaderOpenCV::recycle() {
    this->_im.release();
    this->_width = -1;
    this->_height = -1;
    this->_depth = -1;
    this->_channels = -1;
}


void TiffReaderOpenCV::open(std::string filename) {
    
    cv::Mat raw_im = cv::imread(filename);

    if(raw_im.data == NULL)
        throw ReaderFailedToOpenFile();


    if(raw_im.depth() != CV_16U) {
        this->recycle();
        throw std::runtime_error("[TiffReaderOpenCV] Only 16 bit tiffs are supported.");
    }

    cv::cvtColor(raw_im, this->_im, cv::COLOR_BGR2RGB);

    this->_width = this->_im.cols;
    this->_height = this->_im.rows;
    this->_channels = this->_im.channels();
    this->_depth = 16;
}



void TiffReaderOpenCV::copyBitmapTo(void* buffer, uint32_t size) {

    if( size < _width * _height * _channels * (_depth / 8) )
        throw std::logic_error("[TiffReaderOpenCV] Buffer size is too small.");
    
    memcpy(buffer, this->_im.data, size);

}


void TiffReaderOpenCV::copyBitmapTo(cv::Mat& im) {
    im = this->_im;
}

}