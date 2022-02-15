#include "Image.hpp"


namespace btrgb {


    Image::Image(std::string name) { this->_name = name; }

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



    std::string Image::getName() {
        return this->_name;
    }

    void Image::setName(std::string name) {
        this->_name = name;
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
			throw ImageNotInitialized(this->_name);
    }


    binary_ptr_t Image::toBinaryOfType(enum image_binary_type type, enum image_quality quality) {
        binary_ptr_t result_binary(new std::vector<uchar>);
        std::vector<int> params;

        cv::Mat im;
        switch(quality) {
        case FAST:

            /* Scale width of image to be 2000 pixels. */
            double scaler = double(2000) / double(this->_opencv_mat.cols);
            cv::resize(this->_opencv_mat, im, cv::Size(), scaler, scaler, cv::INTER_AREA);
            im.convertTo(im, CV_8U, 0xFF);

            /* Set compression parameters for use later. */
            switch(type) {
                case PNG: params = {
                    cv::IMWRITE_PNG_COMPRESSION, 1,
                    cv::IMWRITE_PNG_STRATEGY, cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY,
                }; break;

                case WEBP: params = {
                    cv::IMWRITE_WEBP_QUALITY, 1
                }; break;
                
                default: params = { /* Use default values. */
                }; break;
            } 
            break;

        case FULL:

            if(type == WEBP) /* WebP must be 8bit. */
                this->_opencv_mat.convertTo(im, CV_8U, 0xFF);
            else /* PNG & JPEG can be 16 bit. */
                this->_opencv_mat.convertTo(im, CV_16U, 0xFFFF);

            /* Set compression parameters for use later. */
            switch(type) {
                case WEBP: params = { /* Quality above 100 for lossless. */
                    cv::IMWRITE_WEBP_QUALITY, 101
                }; break;

                default: params = { /* Use default values. */
                }; break;
            } 
            break;

        default:
            throw std::logic_error("[Image::getBinaryOfType] Invalid quality type. ");

        }

        try {
            switch(type) {
                /* Write image as png in the result buffer. */
                case PNG: {
                    cv::imencode(".png", im, *result_binary, params);
                }   break;

                /* Write image as webp in the result buffer. */
                case WEBP: {
                    cv::imencode(".webp", im, *result_binary, params);
                }   break;

                default:
                    throw std::logic_error("[Image::getBinaryOfType] Invalid image type. ");
            }
        }
        catch(const cv::Exception& ex) {
            /* Failed to encode image. */
            throw;
        }

        return result_binary;
    }



    base64_ptr_t Image::toBase64OfType(enum image_binary_type type, enum image_quality quality) {

        binary_ptr_t img_bin = this->toBinaryOfType(type, quality);

        base64_ptr_t result_base64(new std::string(
            "data:image/png;base64," + cppcodec::base64_rfc4648::encode(*img_bin)
        ));
        
        return result_base64;
    }

}