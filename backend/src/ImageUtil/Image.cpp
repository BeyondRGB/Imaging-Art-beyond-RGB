#include "Image.hpp"


namespace btrgb {


    Image::Image(std::string filename) { this->_filename = filename; }

    Image::~Image() {
        if (this->_bitmap != nullptr) {
            delete[] this->_bitmap;
            this->_bitmap = nullptr;
        }
    }



    void Image::initImage(int width, int height, int channels) {

        if( channels < 1 || channels > 10)
            throw UnsupportedChannels();

        try {
            this->_bitmap = new float[width * height * channels];
        } catch(const std::bad_alloc& e) {
            /* Not enough memory: 
             * This should be caught by pipeline and ArtObject properly deleted. */
            throw;
        }

        cv::Mat im(width, height, CV_32FC(channels), this->_bitmap);
        
        this->_opencv_mat = im;
        this->_width = width;
        this->_height = height;
        this->_channels = channels;
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


    template<typename T> inline void Image::forEach(const T& foreach_callback) {
        this->_opencv_mat.forEach<float[]>(foreach_callback);
    }
    

    void Image::recycle() {
        if (this->_bitmap != nullptr) {
            delete[] this->_bitmap;
            this->_bitmap == nullptr;
        }
        this->_bitmap = 0;
        this->_width = 0;
        this->_height = 0;
        this->_channels = 0;
        cv::Mat empty;
        this->_opencv_mat = empty;
        int _raw_bit_depth = 0;
        this->_temp.release();
    }

    void inline Image::_checkInit() {
        if (this->_bitmap == nullptr)
			throw ImageNotInitialized(this->_filename);
    }

}
