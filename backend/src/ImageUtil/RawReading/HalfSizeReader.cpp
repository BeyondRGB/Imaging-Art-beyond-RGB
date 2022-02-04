#include "HalfSizeReader.hpp"

namespace btrgb {

    HalfSizeReader::HalfSizeReader() {
        this->_rawReader.imgdata.params.output_bps = 8;
        this->_rawReader.imgdata.params.half_size = 1;
        //this->_rawReader.imgdata.params.user_qual = 0;
        //this->_rawReader.imgdata.params.no_interpolation = 1;
    }

    void HalfSizeReader::open(std::string file) {
        int ec;

        /* Open file. */
        ec = this->_rawReader.open_file(file.c_str());
        if(ec) {
            this->_rawReader.recycle();
            throw std::runtime_error("[HalfSizeReader] Failed to open file.");
        }

        /* Unpack file. */
        ec = this->_rawReader.unpack();
        if(ec) {
            this->_rawReader.recycle();
            throw std::runtime_error("[HalfSizeReader] Failed to unpack image.");
        }

        /* Default image processing. */
        ec = this->_rawReader.dcraw_process();
        if(ec) {
            this->_rawReader.recycle();
            throw std::runtime_error("[HalfSizeReader] Failed to process image.");
        }

        /* Get format of processed image. */
        int bits_per_pixel;
        this->_rawReader.get_mem_image_format(
            &(this->_width), 
            &(this->_height), 
            &(this->_channels), 
            &bits_per_pixel);
        if(bits_per_pixel != 8) {
            this->_rawReader.recycle();
            throw std::logic_error("[HalfSizeReader] Result is not 8 bit.");
        }

    }

    int HalfSizeReader::width(){
        return this->_width;
    }

    int HalfSizeReader::height(){
        return this->_height;
    }

    int HalfSizeReader::channels(){
        return this->_channels;
    }

    void HalfSizeReader::copyBitmap(uint8_t* buffer, uint32_t size) {
        if( size < this->_width * this->_height * this->_channels )
            throw std::logic_error("[HalfSizeReader] Buffer size is too small.");
        
        int stride = this->_width * this->_channels;
        int ec = this->_rawReader.copy_mem_image(buffer, stride, false);
        if(ec) throw std::runtime_error("[HalfSizeReader] Failed to copy image to buffer.");
    }


    void HalfSizeReader::release() {
        this->_rawReader.recycle();
    }

}
