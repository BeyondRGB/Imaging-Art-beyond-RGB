#include "LibRawThumbnail.hpp"

namespace btrgb {

LibRawThumbnail::LibRawThumbnail() {}
LibRawThumbnail::~LibRawThumbnail() {this->_reader.recycle();}

void LibRawThumbnail::open(std::string filename) {
    int error_code;

    /* Open the file. */
    error_code = this->_reader.open_file(filename.c_str());
    if(error_code) {
        this->recycle();
        throw ReaderFailedToOpenFile();
    }

    /* Unpack thumbnail data. */
    error_code = this->_reader.unpack_thumb();
    if(error_code) {
        this->recycle();
        throw std::runtime_error("[LibRaw] Failed to unpack thumbnail.");
    }

    /* Determine thumbnail type. */
    switch(this->_reader.imgdata.thumbnail.tformat) {
        
        case LIBRAW_THUMBNAIL_JPEG:
            this->_is_encoded = true;
            this->_length = this->_reader.imgdata.thumbnail.tlength;
            break;

        case LIBRAW_THUMBNAIL_BITMAP:
            this->_is_encoded = false;
            this->_width = this->_reader.imgdata.thumbnail.twidth;
            this->_channels = this->_reader.imgdata.thumbnail.tcolors;
            this->_height = this->_reader.imgdata.thumbnail.theight;
            this->_depth = 8;
            break;

        default:
            this->recycle();
            throw std::runtime_error("[LibRaw] Unsupported thumbnail.");
    }

    this->_is_open = true;
    this->_data = this->_reader.imgdata.thumbnail.thumb;

}

void LibRawThumbnail::recycle() {
    this->_reader.recycle();
    this->_width = -1;
    this->_height = -1;
    this->_depth = -1;
    this->_channels = -1;
    this->_is_open = false;
}

void LibRawThumbnail::copyBitmapTo(void* buffer, uint32_t size) {}
void LibRawThumbnail::copyBitmapTo(cv::Mat& im) {}

}