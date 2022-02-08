#ifndef BTRGB_LIBPNGWRITER_H
#define BTRGB_LIBPNGWRITER_H

#include <vector>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include <zlib.h>
#include <png.h>
#include <opencv2/opencv.hpp>

#include "ImageUtil/Image.hpp"
#include "ImageWriter.hpp"


namespace btrgb {
    
    class LibpngWriter : public ImageWriter {

        public:
            LibpngWriter();
            ~LibpngWriter();
            void write_png(Image* im, std::string filename, 
                std::vector<uint8_t>* buffer = nullptr,
                int special_input_bit_depth = -1);

        protected:
            void _write(Image* im, std::string filename) override;

    };


    class Libpng_OpenFileFailed : public ImageWritingError {
        public:
        virtual char const * what() const noexcept { return "Libpng Error: Cannot open file for writing."; }
    };

    class Libpng_LibraryInitFailed : public ImageWritingError {
        public:
        virtual char const * what() const noexcept { return "Libpng Error: Library initialization failed."; }
    };

    class Libpng_WriteFailed : public ImageWritingError {
        public:
        virtual char const * what() const noexcept { return "Libpng Error: Could not write image to file."; }
    };
}

#endif
