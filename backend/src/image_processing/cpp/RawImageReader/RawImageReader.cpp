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


void RawImageReader::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Reading In Raw Image Data!");
    
    /* For each btrgb::image struct in the art object. */
    for(const auto& [key, im] : *images) {

        try {
            func("RawImageReader: Loading " + im->filename() + "...");
            this->fileReader->read(im);
        }
        catch(const RawReaderStrategy_FailedToOpenFile) {
            func("RawImageReader: Failed to open raw image.");
            images->deleteImage(key);
        }

    }
}