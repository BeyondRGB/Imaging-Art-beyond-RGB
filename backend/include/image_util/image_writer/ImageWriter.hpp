#ifndef IMAGEWRITERSTRATEGY_H
#define IMAGEWRITERSTRATEGY_H

#include <string>

#include <image_util/Image.hpp>
#include <image_util/image_writer/ImageWriterStrategy.hpp>

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