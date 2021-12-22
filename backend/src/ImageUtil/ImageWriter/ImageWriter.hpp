#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include <string>
#include "ImageUtil/Image.hpp"

namespace btrgb {
    
    class ImageWriter {
        public:
            ImageWriter() {}
            ~ImageWriter() {}
            virtual void write(image* im, std::string& filename) = 0;
            virtual void write(image* im) = 0;

    };

    class ImageWritingError : public std::exception {};
}

#endif