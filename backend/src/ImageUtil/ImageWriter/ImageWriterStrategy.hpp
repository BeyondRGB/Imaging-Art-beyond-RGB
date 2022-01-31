#ifndef IMAGEWRITERSTRATEGY_H
#define IMAGEWRITERSTRATEGY_H

#include <string>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/ImageWriter.hpp"
#include "ImageUtil/ImageWriter/LibTiffWriter.hpp"
#include "ImageUtil/ImageWriter/LibpngWriter.hpp"

namespace btrgb {

    enum output_type {
        TIFF,
        PNG
    };
    
    class ImageWriterStrategy {
        
        public:
            ImageWriterStrategy(enum output_type file_type);
            ~ImageWriterStrategy();
            void write(image* im, std::string filename);
            void write(image* im);

        private:
            ImageWriter* writer;

    };

}

#endif