#ifndef LIBPNGWRITER_H
#define LIBPNGWRITER_H

#include <string>
#include <stdio.h>

#include <zlib.h>
#include <png.h>

#include "ImageUtil/Image.hpp"
#include "ImageWriter.hpp"


namespace btrgb {
    
    class LibpngWriter : public ImageWriter {
        public:
            LibpngWriter();
            ~LibpngWriter();
        protected:
            void _write(image* im, std::string filename) override;
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