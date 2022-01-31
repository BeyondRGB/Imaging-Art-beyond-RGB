#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include <string>
#include "ImageUtil/Image.hpp"

namespace btrgb {
    
    class ImageWriter {
        
        public:
            ImageWriter() {}
            virtual ~ImageWriter() {}
            virtual void write(image* im, std::string filename);
            virtual void write(image* im);

        protected:
            std::string file_extension;
            virtual void _write(image* im, std::string filename) = 0;

    };

    class ImageWritingError : public std::exception {};
}

#endif