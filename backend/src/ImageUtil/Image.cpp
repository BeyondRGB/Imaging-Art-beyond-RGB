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
        this->_color_profile = none;
    }

    void inline Image::_checkInit() {
        if (this->_bitmap == nullptr)
			throw ImageNotInitialized(this->_name);
    }


    binary_ptr_t Image::getEncodedPNG(enum image_quality quality) {
        std::vector<int> params;
        cv::Mat im;

        /* Convert color space. */
        cv::Mat im_srgb = Image::copyMatConvertDepth(this->_opencv_mat, CV_32F);
        ColorProfiles::convert(im_srgb, this->_color_profile, ColorSpace::sRGB);

        switch(quality) {
        case FAST:

            /* Convert to 8 bit. */
            {cv::Mat im8u = Image::copyMatConvertDepth(im_srgb, CV_8U);
            im_srgb.release();

            /* Scale the image to have a width of 1920 pixels (keep same aspect ratio). */
            if(im8u.cols > 1920) {
                double scaler = double(1920) / double(im8u.cols);
                cv::resize(im8u, im, cv::Size(), scaler, scaler, cv::INTER_AREA);
            }
            else {
                im = im8u;
            }}
        
            /* Set compression parameters for use later. */
            params = {
                cv::IMWRITE_PNG_COMPRESSION, 1,
                cv::IMWRITE_PNG_STRATEGY, cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY,
            };
            break;

        case FULL:
        
            /* Convert to 16 bit. */
            im = Image::copyMatConvertDepth(im_srgb, CV_16U);
            im_srgb.release();

            /* Use default PNG compression parameters. */
            params = {};
            break;

        default:
            throw std::logic_error("[Image::getBinaryOfType] Invalid quality type. ");
        }

        /* Convert to BGR order for OpenCV. */
        cv::Mat im_bgr;
        cv::cvtColor(im, im_bgr, cv::COLOR_RGB2BGR);

        /* Encode image. */
        binary_ptr_t result_binary(new std::vector<uchar>);
        try { cv::imencode(".png", im_bgr, *result_binary, params); }
        /* Failed to encode image. */
        catch(const cv::Exception& ex) {
            throw FailedToEncode();
        }

        return result_binary;
    }



    void Image::setColorProfile(ColorSpace color_profile) {
        this->_color_profile = color_profile;
    }

    ColorSpace Image::getColorProfile() {
        return this->_color_profile;
    }


    /* ====== static ======= */
    bool Image::is_tiff(std::string filename) {
        if( !fs::is_regular_file(filename) ) 
            return false;

        std::string::size_type i = filename.rfind('.');
        if( i == std::string::npos ) 
            return false;

        std::string ext = filename.substr(i + 1);
        return ext == "tiff" || ext == "tif";
    }
            

    /* ====== static ======= */
    cv::Mat Image::copyMatConvertDepth(cv::Mat input, int cv_depth) {

        double current_max, target_max;

        switch(cv_depth) {
            case CV_32F: case CV_64F: target_max = 1; break;
            case CV_16U: target_max = 0xFFFF; break;
            case CV_8U: target_max = 0xFF; break;
            default: throw std::runtime_error("[Image::convertCopyMat] Unsupported target depth.");
        }

        switch(input.depth()) {
            case CV_32F: case CV_64F: current_max = 1; break;
            case CV_16U: current_max = 0xFFFF; break;
            case CV_8U: current_max = 0xFF; break;
            default: throw std::runtime_error("[Image::convertCopyMat] Unsupported current image depth.");
        }

        cv::Mat result;
        input.convertTo(result, cv_depth, target_max / current_max);
        return result;
    }

}