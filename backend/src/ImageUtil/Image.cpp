#include "Image.hpp"


namespace btrgb {


    Image::Image(std::string filename) { this->_filename = filename; }

    Image::~Image() {
        /* Shouldn't have an effect, but it will help debug if
         * someone is accidentally holding onto a reference and then use it. */
        this->_opencv_mat.release();
    }



    void Image::initImage(cv::Mat im) {
        this->_opencv_mat = im;
        this->_bitmap = (float*) im.data;
        this->_width = im.cols;
        this->_height = im.rows;
        this->_channels = im.channels();
    }



    std::string Image::filename() {
        return this->_filename;
    }

    void Image::setFilename(std::string filename) {
        this->_filename = filename;
    }



    int Image::width() {
        _checkInit();
        return this->_width;
    }

    int Image::height() {
        _checkInit();
        return this->_height;
    }

    int Image::channels() {
        _checkInit();
        return this->_channels;
    }



    float* Image::bitmap() {
        _checkInit();
        return this->_bitmap;
    }

    cv::Mat Image::getMat() {
        _checkInit();
        return this->_opencv_mat;
    }
            

    uint32_t Image::getIndex(int row, int col, int ch) {
        return row * this->_width * this->_channels + col * this->_channels + ch;
    }

    
    void Image::setPixel(int row, int col, int ch, float value) {
        this->_bitmap[row * this->_width * this->_channels + col * this->_channels + ch] = value;
    }

    float Image::getPixel(int row, int col, int ch) {
        return this->_bitmap[row * this->_width * this->_channels + col * this->_channels + ch];
    }

    float* Image::getPixelPointer(int row, int col) {
        return &( this->_bitmap[row * this->_width * this->_channels + col * this->_channels] );
    }
    

    void Image::recycle() {
        this->_bitmap = nullptr;
        this->_width = 0;
        this->_height = 0;
        this->_channels = 0;
        this->_opencv_mat.release();
        cv::Mat empty;
        this->_opencv_mat = empty;
        int _raw_bit_depth = 0;
    }

    void inline Image::_checkInit() {
        if (this->_bitmap == nullptr)
			throw ImageNotInitialized(this->_filename);
    }

}
