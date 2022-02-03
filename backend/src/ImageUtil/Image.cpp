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
            

    uint32_t inline Image::getIndex(int row, int col, int ch) {
        return row * this->_width * this->_channels + col * this->_channels + ch;
    }

    
    void inline Image::setPixel(int row, int col, int ch, float value) {
        this->_bitmap[row * this->_width * this->_channels + col * this->_channels + ch] = value;
    }

    float inline Image::getPixel(int row, int col, int ch) {
        return this->_bitmap[row * this->_width * this->_channels + col * this->_channels + ch];
    }

    float inline* Image::getPixelPointer(int row, int col) {
        return &( this->_bitmap[row * this->_width * this->_channels + col * this->_channels] );
    }

    /* Get array of channels for each pixel: (Use constants at end of Image.hpp)

    im->forEach([](float (&pixel)[], const int* pos) -> void {
        pixel[R] = 5;
        float blue = pixel[B];
        float blue1 = pixel[B1]; // B is the same as B1 so blue == blue1
        float green2 = pixel[G2]; // Only use R2, B2, & G2 for six channel images
    })


    * Loop through every channel for each pixel: (put number of channels in capture list)

    int channels = im->channels();
    im->forEach( [channels](float (&pixel)[], const int* pos) -> void {
        for( int ch = 0; ch < channels; ch++) {
            pixel[ch] = <whatever value>;
            <whatever variable> = pixel[ch];
        }
    });

    */
    template<typename T> inline void Image::forEach(const T& foreach_callback) {
        this->_opencv_mat.forEach<float[]>(foreach_callback);
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
