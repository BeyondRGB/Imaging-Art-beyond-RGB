#include "Image.hpp"

namespace btrgb {

    image::image(std::string filename) {
        this->_filename = filename;
        this->_bitmap = 0;
        this->_width = 0;
        this->_height = 0;
        this->_channels = 0;
    }

    image::~image() {
        if (this->_bitmap)
            delete this->_bitmap;
    }

    void image::initBitmap(int width, int height, int channels) {
        try {
            this->_bitmap = new pixel[width * height * channels];
        } catch (std::bad_alloc&) {
            throw;
        }

        this->_width = width;
        this->_height = height;
        this->_channels = channels;
    }

    std::string image::filename() {
        return this->_filename;
    }

    void image::setFilename(std::string filename) {
        this->_filename = filename;
    }

    int image::width() {
        checkInit();
        return this->_width;
    }

    int image::height() {
        checkInit();
        return this->_height;
    }

    int image::channels() {
        checkInit();
        return this->_channels;
    }

    pixel* image::bitmap() {
        checkInit();
        return this->_bitmap;
    }
            
    uint32_t image::getIndex(int row, int col, int ch) {
        return row * _width * _channels + col * _channels + ch;
    }
    
    void image::setPixel(int row, int col, int ch, btrgb::pixel value) {
        _bitmap[row * _width * _channels + col * _channels + ch] = value;
    }

    btrgb::pixel image::getPixel(int row, int col, int ch) {
        return _bitmap[row * _width * _channels + col * _channels + ch];
    }

    void image::recycle() {
        if (this->_bitmap)
            delete this->_bitmap;
        this->_bitmap = 0;
        this->_width = 0;
        this->_height = 0;
        this->_channels = 0;
    }

    void image::checkInit() {
        if (! this->_bitmap)
			throw BitmapNotInitialized(this->_filename);
    }

}
