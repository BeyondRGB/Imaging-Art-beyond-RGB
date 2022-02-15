#include "HalfSizeReader.hpp"

namespace btrgb {

    HalfSizeReader::HalfSizeReader() {
	    libraw_output_params_t* opt = &( this->_reader.imgdata.params );
        opt->gamm[0] = 1.0;
        opt->gamm[1] = 1.0;
        opt->output_bps = 8;
        opt->half_size = 1;
        opt->use_camera_wb = 0;
        opt->use_auto_wb = 0;
        opt->use_camera_matrix = 0;
        opt->output_color = 0;
        opt->use_rawspeed = 1;
        opt->user_qual = 0;
        opt->no_interpolation = 1;
    }

    void HalfSizeReader::open(std::string file) {
        int error_code;

        /* Open file. */
        error_code = this->_reader.open_file(file.c_str());
        if(error_code) {
            this->_reader.recycle();
            throw std::runtime_error("[HalfSizeReader] Failed to open file.");
        }

        /* Unpack file. */
        error_code = this->_reader.unpack();
        if(error_code) {
            this->_reader.recycle();
            throw std::runtime_error("[HalfSizeReader] Failed to unpack image.");
        }

        /* Default image processing. */
        error_code = this->_reader.dcraw_process();
        if(error_code) {
            this->_reader.recycle();
            throw std::runtime_error("[HalfSizeReader] Failed to process image.");
        }

        /* Get format of processed image. */
        int bits_per_pixel;
        this->_reader.get_mem_image_format(
            &(this->_width), 
            &(this->_height), 
            &(this->_channels), 
            &bits_per_pixel);
        if(bits_per_pixel != 8) {
            this->_reader.recycle();
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
        int error_code = this->_reader.copy_mem_image(buffer, stride, false);
        if(error_code) throw std::runtime_error("[HalfSizeReader] Failed to copy image to buffer.");
    }


    void HalfSizeReader::release() {
        this->_reader.recycle();
    }

}
