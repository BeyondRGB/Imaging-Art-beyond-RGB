#ifndef TIFFWRITER_H
#define TIFFWRITER_H

extern "C" {
    #include <tiffio.h>
}
#include <string>
#include <opencv2/opencv.hpp>

#include "ImageUtil/Image.hpp"
#include "ImageWriter.hpp"

namespace btrgb {
    
    class LibTiffWriter: public ImageWriter {
        public:
            LibTiffWriter();
            ~LibTiffWriter();
        protected:
            void _write(Image* im, std::string filename) override;
    };

    class LibTiff_OpenFileFailed : public ImageWritingError {
        public:
        virtual char const * what() const noexcept { return "LibTiff Error: Cannot open file for writing."; }
    };

    class LibTiff_WriteStripFailed : public ImageWritingError {
        public:
        virtual char const * what() const noexcept { return "LibTiff Error: Cannot write strip buffer to file."; }
    };
}

#endif