#ifndef BTRGB_IMAGEREADERSTRATEGY_HPP
#define BTRGB_IMAGEREADERSTRATEGY_HPP

#include <stdint.h>
#include <string>
#include <opencv2/opencv.hpp>

#include "btrgb.hpp"

namespace btrgb {
    
class ImageReaderStrategy {

    public:
        virtual ~ImageReaderStrategy() {}
        virtual void open(std::string filename) = 0;
        virtual void recycle() = 0;

        int width() {return _width;}
        int height() {return _height;}
        int channels() {return _channels;}
        int depth() {return _depth;}
        exif getExifData() {return this->_tags;}

        virtual void copyBitmapTo(void* buffer, uint32_t size) = 0;
        virtual void copyBitmapTo(cv::Mat& im) = 0;

        int getCVMatType() {
            switch(_depth) {
                case  8: return CV_8U;
                case 16: return CV_16U;
                default: throw std::runtime_error("[ImageReaderStrategy] Unsupported bit-depth.");
            }
        }


    protected:
        int _width = -1;
        int _height = -1;
        int _channels = -1;
        int _depth = -1;
        bool _is_open = false;
        exif _tags;

};

class ImageReaderStrategyError : public std::exception {};
    
class ReaderFailedToOpenFile : public ImageReaderStrategyError {
    public:
    virtual char const * what() const noexcept { return "[ImageReaderStrategy] Failed to open file."; }
};

}

#endif