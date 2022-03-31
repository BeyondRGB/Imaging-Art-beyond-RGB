#ifndef IMAGEWRITERSTRATEGY_H
#define IMAGEWRITERSTRATEGY_H

#include <string>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/ImageWriterStrategy.hpp"
#include "ImageUtil/ImageWriter/LibTiffWriter.hpp"
#include "ImageUtil/ImageWriter/LibpngWriter.hpp"

namespace btrgb {
    
    class ImageWriter {
        
        public:
            ImageWriter(enum output_type file_type);
            ~ImageWriter();
            void write(Image* im, std::string filename);
            void write(Image* im);

        private:
            ImageWriterStrategy* writer;

    };

}

#endif