#ifndef IMAGEWRITERSTRATEGY_H
#define IMAGEWRITERSTRATEGY_H

#include <string>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/ImageWriter.hpp"
#include "ImageUtil/ImageWriter/LibTiffWriter.hpp"
#include "ImageUtil/ImageWriter/LibpngWriter.hpp"

namespace btrgb {
    
    class ImageWriterStrategy {
        
        public:
            ImageWriterStrategy(enum output_type file_type);
            ~ImageWriterStrategy();
            void write(Image* im, std::string filename);
            void write(Image* im);

        private:
            ImageWriter* writer;

    };

}

#endif