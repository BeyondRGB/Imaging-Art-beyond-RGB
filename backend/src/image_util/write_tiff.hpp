#ifndef WRITE_TIFF_HPP
#define WRITE_TIFF_HPP

#include "../btrgb.hpp"

extern "C" {
    #include <tiffio.h>
}

namespace tiff_util {

    typedef enum {
        none,
        cannot_open_for_writing,
        failed_to_write_strip
    } status;

    status write(btrgb::image* data);

    /* 
     * To do?
     * Init function to call TIFFSetErrorHandler(0) to supress default
     * error handler which prints to stderr.
    */

};

#endif