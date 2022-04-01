#include "LibTiffReader.hpp"

namespace btrgb {

LibTiffReader::LibTiffReader() {

}


LibTiffReader::~LibTiffReader() {
    if(this->_is_open)
	    TIFFClose(this->_tiff);
}


void LibTiffReader::recycle() {
    if(this->_is_open)
	    TIFFClose(this->_tiff);
    this->_is_open = false;
    this->_width = -1;
    this->_height = -1;
    this->_depth = -1;
    this->_channels = -1;
}



void LibTiffReader::open(std::string filename) {
    this->_tiff = TIFFOpen(filename.c_str(), "r");
    if(! this->_tiff)
        throw std::runtime_error("[LibTiffReader] Failed to access file: " + filename);
    this->_is_open = true;

    uint16_t channels, depth;
    TIFFGetField(this->_tiff, TIFFTAG_IMAGEWIDTH, &(this->_width));
    TIFFGetField(this->_tiff, TIFFTAG_IMAGELENGTH, &(this->_height));
    TIFFGetField(this->_tiff, TIFFTAG_SAMPLESPERPIXEL, &channels);
    TIFFGetField(this->_tiff, TIFFTAG_BITSPERSAMPLE, &depth);
    this->_channels = channels;
    this->_depth = depth;

    if( 
        _width <= 0 ||
        _height <= 0 ||
        _channels <= 0 ||
        !(_depth == 8 || _depth == 16)
    ) {
        this->recycle();
        throw std::runtime_error("[LibTiffReader] Invalid image dimensions.");
    }
}



void LibTiffReader::copyBitmapTo(void* buffer, uint32_t size) {
    if( size < _width * _height * _channels * (_depth / 8) )
        throw std::logic_error("[LibTiffReader] Buffer size is too small.");

    std::cout << "Starting copy\n";
    uint32_t strip_data_size = TIFFStripSize(this->_tiff);
    std::cout << "Got strip size\n";
	void* strip_data = _TIFFmalloc(strip_data_size);
    if( ! strip_data )
        throw std::runtime_error("[LibTiffReader] Memory error.");
    std::cout << "Allocate buffer\n";

	uint32_t strip_index;
	uint32_t strip_size;
    uint32_t total_strips = TIFFNumberOfStrips(this->_tiff);

	for (strip_index = 0; strip_index < total_strips; strip_index++) {
        std::cout << "Strip #" << strip_index << std::endl;
		strip_size = TIFFReadEncodedStrip(this->_tiff, strip_index, strip_data, strip_data_size);
        if(strip_size < 0) {
	        _TIFFfree(strip_data);
            throw std::runtime_error("[LibTiffReader] Failed to read strip #" + std::to_string(strip_index));
        }
        memcpy(buffer, strip_data, strip_size);
        buffer = (char*)buffer + strip_size;
    }
	_TIFFfree(strip_data);
}


void LibTiffReader::copyBitmapTo(cv::Mat& im) {

    /* Get depth of image to create. */
    int cv_depth;
    switch(this->_depth) {
        case  8: cv_depth = CV_8U; break;
        case 16: cv_depth = CV_16U; break;
        default: throw std::runtime_error("[LibTiffReader] Unsupported bit-depth.");
    }
    
    /* Copy LibTiff image data to OpenCV Mat. */
    cv::Mat raw_im(_height, _width, CV_MAKETYPE(cv_depth, _channels));
    this->copyBitmapTo(raw_im.data, _width * _height * _channels * (_depth / 8));
 
    im = raw_im;
}

}