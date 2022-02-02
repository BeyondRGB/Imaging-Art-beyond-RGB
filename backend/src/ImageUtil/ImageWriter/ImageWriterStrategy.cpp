#include "ImageWriterStrategy.hpp"

namespace btrgb {

    ImageWriterStrategy::ImageWriterStrategy(enum output_type file_type) {
        switch(file_type) {

            case PNG:
                this->writer = new LibpngWriter();
                break;
            
            default:
            case TIFF:
                this->writer = new LibTiffWriter();
                break;

        }
    }

    ImageWriterStrategy::~ImageWriterStrategy() {
        delete this->writer;
    }

    void ImageWriterStrategy::write(image* im, std::string filename) {
        this->writer->write(im, filename);
    }
    
    void ImageWriterStrategy::write(image* im) {
        this->writer->write(im);
    }

}
