#include <string>

#include "ImageUtil/Image.hpp"
#include "image_processing/header/LibRawReader.h"
#include "image_processing/header/RawImageReader.h"


RawImageReader::RawImageReader() {
    this->fileReader = new LibRawReader();
}


RawImageReader::RawImageReader(std::string strategy) {
    if (strategy == "LibRaw") {
        this->fileReader = new LibRawReader();
    }
    else { /* default to LibRaw */
        this->fileReader = new LibRawReader();
    }
}


RawImageReader::~RawImageReader() {
    delete this->fileReader;
}


void RawImageReader::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Reading In Raw Image Data!", "RawImageReader");

    /* For each btrgb::image struct in the art object. */
    for(const auto& [key, im] : *images) {

        try {
            comms->send_info("Loading " + im->filename() + "...", "RawImageReader");
            if(key == "white1") {
                this->fileReader->read(im, RawReaderStrategy::RECORD_BIT_DEPTH);
            }
            else {
                this->fileReader->read(im);
            }
        }
        catch(const RawReaderStrategy_FailedToOpenFile) {
            comms->send_error("Failed to open raw image.", "RawImageReader");
            images->deleteImage(key);
        }

    }


    btrgb::Image* white1 = images->getImage("white1");
    for(const auto& [key, im] : *images) {
        if(key != "white1")
            im->_raw_bit_depth = white1->_raw_bit_depth;
    }


    //Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "RawReadOut1");
    images->outputImageAs(btrgb::TIFF, "art2", "RawReadOut2");
}
