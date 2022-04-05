#ifndef BTRGB_LIBTIFFREADER_HPP
#define BTRGB_LIBTIFFREADER_HPP

extern "C" {
    #include <tiffio.h>
}

#include <jsoncons/json.hpp>
#include "ImageReaderStrategy.hpp"

namespace btrgb {

class LibTiffReader : public ImageReaderStrategy {

    public:
        LibTiffReader();
        ~LibTiffReader();

        void open(std::string filename) override;
        void recycle() override;

        void copyBitmapTo(void* buffer, uint32_t size) override;
        void copyBitmapTo(cv::Mat& im) override;

        cv::Mat getCrop(uint32_t left, uint32_t right, uint32_t width, uint32_t height);
        std::unordered_map<std::string, cv::Mat> getConversionMatrices();
        cv::Mat getConversionMatrix(std::string key);
        std::string getColorProfileString();

    private:
        struct tiff* _tiff;
        cv::Mat _extractMat(jsoncons::json matJson);
};

}

#endif