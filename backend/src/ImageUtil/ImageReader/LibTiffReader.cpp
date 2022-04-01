#include "LibTiffReader.hpp"

namespace btrgb {

LibTiffReader::LibTiffReader() {

}


LibTiffReader::~LibTiffReader() {

}


void LibTiffReader::recycle() {

}



void LibTiffReader::open(std::string filename) {
   
}



void LibTiffReader::copyBitmapTo(void* buffer, uint32_t size) {
    if( size < _width * _height * _channels * (_depth / 8) )
        throw std::logic_error("[LibTiffReader] Buffer size is too small.");
    
}


void LibTiffReader::copyBitmapTo(cv::Mat& im) {

    /* Get depth of image to create. */
    int cv_depth;
    switch(this->_depth) {
        case  8: cv_depth = CV_8U; break;
        case 16: cv_depth = CV_16U; break;
        default: throw std::runtime_error("[LibTiffReader] Internal error.");
    }
    
    /* Copy LibTiff image data to OpenCV Mat. */
    cv::Mat raw_im(_height, _width, CV_MAKETYPE(cv_depth, _channels));
 







 
    im = raw_im;

}

}