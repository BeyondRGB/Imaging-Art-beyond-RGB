#ifndef BTRGB_HALFSIZEREADER_HPP
#define BTRGB_HALFSIZEREADER_HPP

#include <stdint.h>
#include <string>
#include <math.h>
#include <memory>
#include <libraw.h>
#include <opencv2/opencv.hpp>

namespace btrgb {


    class HalfSizeReader {

        public:
            HalfSizeReader();
            void open(std::string file);
            int width();
            int height();
            int channels();
            void copyBitmap(uint8_t* buffer, uint32_t size);
            void release();

        private:
            int _width = -1;
            int _height = -1;
            int _channels = -1;
            LibRaw _reader;
    };


}


#endif
