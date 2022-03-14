#include "ImageWriterStrategy.hpp"

namespace btrgb {

    void ImageWriterStrategy::write(Image* im) {
        write(im, im->getName());
    }

    void ImageWriterStrategy::write(Image* im, std::string filename) {

        std::string fname = filename;
        if (!fname.ends_with(this->file_extension))
            fname += this->file_extension;

        this->_write(im, fname);

    }

}
