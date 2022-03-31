#include "ImageWriter.hpp"

namespace btrgb {

    ImageWriter::ImageWriter(enum output_type file_type) {
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

    ImageWriter::~ImageWriter() {
        delete this->writer;
    }

    void ImageWriter::write(Image* im, std::string filename) {
        this->writer->write(im, filename);
    }
    
    void ImageWriter::write(Image* im) {
        this->writer->write(im);
    }

}
