#include <string>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/BitDepthFinder.hpp"
#include "ImageUtil/ImageReader/LibRawReader.hpp"
#include "image_processing/header/ImageReader.h"


ImageReader::ImageReader(ImageReader::reader_strategy strategy) {
    switch(strategy) {

        case TIFF_OpenCV:
            // to do
            break;

        case RAW_LibRaw:
            this->_reader = new btrgb::LibRawReader;
            break;

        default: 
            throw std::logic_error("[ImageReader] Invalid strategy.");
    }
}


ImageReader::~ImageReader() {
    delete this->_reader;
}


void ImageReader::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Reading In Raw Image Data!", "ImageReader");

    btrgb::BitDepthFinder util;
    int bit_depth = -1;


    for(const auto& [key, im] : *images) {
        comms->send_info("Loading " + im->getName() + "...", "ImageReader");

        try {
            cv::Mat raw_im;
            _reader->open(im->getName());
            _reader->copyBitmapTo(raw_im);
            _reader->recycle();

            if(raw_im.depth() != CV_16U)
                throw std::runtime_error(" Image must be 16 bit." );


            /* Find bit depth if image is white field #1. */
            if(key == "white1") {
                bit_depth = util.get_bit_depth(
                    (uint16_t*) raw_im.data,    
                    raw_im.cols, 
                    raw_im.rows,
                    raw_im.channels()
                );
                if(bit_depth < 0)
                    throw std::runtime_error(" Bit depth detection of 'white1' failed." );
            }

            /* Convert to floating point. */
            cv::Mat float_im;
            raw_im.convertTo(float_im, CV_32F, 1.0/0xFFFF);

            /* Init btrgb::Image object. */
            im->initImage(float_im);

        }
        catch(const btrgb::ReaderFailedToOpenFile& e) {
            comms->send_error(std::string(e.what()) + " || " + im->getName(), "ImageReader");
            images->deleteImage(key);
        }
        catch(const std::runtime_error& e) {
            comms->send_error(std::string(e.what()) + " || " + im->getName(), "ImageReader");
            images->deleteImage(key);
        }


    }


    for(const auto& [key, im] : *images) {
        im->_raw_bit_depth = bit_depth;
        //images->outputImageAs(btrgb::TIFF, key, key + "_raw");
    }

}
