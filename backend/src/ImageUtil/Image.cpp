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
        this->_col_size = this->_channels;
        this->_row_size = this->_width * this->_col_size;
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
        return row * _row_size + col * _col_size + ch;
    }

    
    void Image::setPixel(int row, int col, int ch, float value) {
        _bitmap[ (row * _row_size) + (col * _col_size) + ch] = value;
    }

    float Image::getPixel(int row, int col, int ch) {
        return _bitmap[ (row * _row_size) + (col * _col_size) + ch];
    }

    float* Image::getPixelPointer(int row, int col) {
        return &( _bitmap[ (row * _row_size) + (col * _col_size) ] );
    }
    

    void Image::recycle() {
        this->_bitmap = nullptr;
        this->_width = 0;
        this->_height = 0;
        this->_channels = 0;
        this->_row_size = 0;
        this->_col_size = 0;
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
