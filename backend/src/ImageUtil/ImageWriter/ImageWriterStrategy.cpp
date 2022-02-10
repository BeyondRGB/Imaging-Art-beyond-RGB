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

    void ImageWriterStrategy::write(Image* im, std::string filename) {
        this->writer->write(im, filename);
    }
    
    void ImageWriterStrategy::write(Image* im) {
        this->writer->write(im);
    }

}
