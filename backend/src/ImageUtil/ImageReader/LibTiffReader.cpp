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
    if(this->_is_open)
        this->recycle();

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

    if(channels == 1) {
        uint16_t planar_config;
        TIFFGetField(this->_tiff, TIFFTAG_PLANARCONFIG, &planar_config);
        if(planar_config != PLANARCONFIG_CONTIG)
            throw std::runtime_error("[LibTiffReader] Only 'Chunky' planar config format is supported: " + filename);
    }

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
    if( ! this->_is_open )
        throw std::runtime_error("[LibTiffReader] No file opened.");

    if( size < _width * _height * _channels * (_depth / 8) )
        throw std::logic_error("[LibTiffReader] Buffer size is too small.");

    uint32_t strip_data_size = TIFFStripSize(this->_tiff);
	void* strip_data = _TIFFmalloc(strip_data_size);
    if( ! strip_data )
        throw std::runtime_error("[LibTiffReader] Memory error.");

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
    if( ! this->_is_open )
        throw std::runtime_error("[LibTiffReader] No file opened.");

    /* Get depth of image to create. */
    int cv_depth = this->getCVMatType();
    
    /* Copy LibTiff image data to OpenCV Mat. */
    cv::Mat raw_im(_height, _width, CV_MAKETYPE(cv_depth, _channels));
    this->copyBitmapTo(raw_im.data, _width * _height * _channels * (_depth / 8));
 
    im = raw_im;
}


cv::Mat LibTiffReader::getCrop(uint32_t left, uint32_t top, uint32_t w, uint32_t h) {
    if( ! this->_is_open )
        throw std::runtime_error("[LibTiffReader] No file opened.");

    int right = left + w;
    int bottom = top + h;

    bool lt_valid = (left >= 0) && (top >= 0) && (left < _width, top < _height);
    bool rb_valid = (right >= 0) && (bottom >= 0) && (right < _width, bottom < _height);
    if( ! (lt_valid && rb_valid) )
        throw std::runtime_error("[LibTiffReader] Invalid coordinates.");

    int sample_byte_size = this->_depth / 2;

    int cv_depth = this->getCVMatType();
    cv::Mat cropped(h, w, CV_MAKETYPE(cv_depth, _channels));


    uint32_t rows_per_strip = 0;
    TIFFGetField(this->_tiff, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    if(rows_per_strip != 1)
        throw std::runtime_error("[LibTiffReader] This tiff was not saved with one row per strip.");


    uint32_t row_size = TIFFStripSize(this->_tiff);
    if(row_size != _width * _channels * sample_byte_size)
        throw std::runtime_error("[LibTiffReader] Strip size assumption failed.");


	void* row_data = _TIFFmalloc(row_size);
    if( ! row_data )
        throw std::runtime_error("[LibTiffReader] Memory error.");


	uint32_t row_index;
	uint32_t row_size;
    uint32_t total_strips = TIFFNumberOfStrips(this->_tiff);
    if( total_strips != this->_height)
        throw std::runtime_error("[LibTiffReader] Strip count assumption failed.");


    void* crop_row = (char*)row_data + (left * _channels * sample_byte_size);
    int crop_row_size = w * _channels * sample_byte_size;
    void* mat_row = cropped.data;
	for (row_index = top; row_index <= bottom; row_index++) {

		row_size = TIFFReadEncodedStrip(this->_tiff, row_index, row_data, row_size);

        if(row_size < 0) {
	        _TIFFfree(row_data);
            throw std::runtime_error("[LibTiffReader] Failed to get row #" + std::to_string(row_index));
        }

        memcpy(mat_row, crop_row, crop_row_size);
        mat_row = (char*)mat_row + crop_row_size;
    }

	_TIFFfree(row_data);
}


}