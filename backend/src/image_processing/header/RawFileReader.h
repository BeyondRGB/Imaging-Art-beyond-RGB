#ifndef BEYOND_RGB_RAWFILEREADER_H
#define BEYOND_RGB_RAWFILEREADER_H

#include "ImageUtil/Image.hpp"

class RawFileReader {
    public:
        virtual ~RawFileReader() {}
        virtual void read(btrgb::image* im) = 0;
};

class RawFileReaderError : public std::exception {};
    
class RawFileReader_FailedToOpenFile : public RawFileReaderError {
    public:
    virtual char const * what() const noexcept { return "RawFileReader Error: Unable to open file."; }
};

#endif