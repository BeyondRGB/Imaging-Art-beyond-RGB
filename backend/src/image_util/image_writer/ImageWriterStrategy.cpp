#include <image_util/image_writer/ImageWriterStrategy.hpp>

namespace btrgb {

void ImageWriterStrategy::write(Image *im, std::string filename) {

    std::string fname;
    if (filename == "")
        fname = im->getName();
    else
        fname = filename;

    if (!fname.ends_with(this->file_extension))
        fname += this->file_extension;

    this->_write(im, fname);
}

} // namespace btrgb
