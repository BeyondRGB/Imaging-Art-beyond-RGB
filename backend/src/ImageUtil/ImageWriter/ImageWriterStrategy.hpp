#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include <string>
#include "ImageUtil/Image.hpp"

namespace btrgb {
    
    class ImageWriterStrategy {
        
        public:
            ImageWriterStrategy() {}
            virtual ~ImageWriterStrategy() {}
            virtual void write(Image* im, std::string filename = "");

        protected:
            std::string file_extension;
            virtual void _write(Image* im, std::string filename) = 0;

    };

    class ImageWritingError : public std::exception {};
}

#endif