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

    int image::width() {
        return this->_width;
    }

    int image::height() {
        return this->_height;
    }

    int image::channels() {
        return this->_channels;
    }

    pixel* image::bitmap() {
        return this->_bitmap;
    }

    void image::recycle() {
        if (this->_bitmap)
            delete this->_bitmap;
        this->_bitmap = 0;
        this->_width = 0;
        this->_height = 0;
        this->_channels = 0;
    }

}
