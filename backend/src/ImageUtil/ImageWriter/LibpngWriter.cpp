#include "LibpngWriter.hpp"

namespace btrgb {
    
    LibpngWriter::LibpngWriter() {this->file_extension = ".png";}
    LibpngWriter::~LibpngWriter() {}

    void LibpngWriter::_write(Image* im, std::string filename) {
        this->write_png(im, filename);
    }

    void LibpngWriter::write_png(Image* im, std::string filename, 
    std::vector<uint8_t>* buffer, 
    int special_input_bit_depth) {

        int height = im->height();
        int width = im->width();
        int channels = im->channels();

        cv::Mat im_8u;
		im->getMat().convertTo(im_8u, CV_8U);
        uint8_t* bitmap = (uint8_t*) im_8u.data;


        bool write_to_file = (buffer == nullptr);
        FILE* output_file;

        if( write_to_file ) {
            /* ============[ Open file for writing ]============== */
            output_file = fopen(filename.c_str(), "wb");
            if(output_file == NULL)
                throw Libpng_OpenFileFailed();
        }

        /* ============[ Create png_struct ]============== */
        png_structp png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING, 
            (png_voidp) NULL /*user_error_ptr*/,
            NULL /*user_error_fn*/, 
            NULL /*user_warning_fn*/
            );
        if (!png_ptr) {
            if( write_to_file )
                fclose(output_file);
            throw Libpng_LibraryInitFailed();
        }

        /* ============[ Create png_info ]============== */
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            if( write_to_file )
                fclose(output_file);
            throw Libpng_LibraryInitFailed();
        }

        /* ============[ Set up error jumping? ]============== */
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            if( write_to_file )
                fclose(output_file);
            throw Libpng_LibraryInitFailed();
        }

        if ( write_to_file ) {
            /* ============[ Set output file for png ]============== */
            png_init_io(png_ptr, output_file);
        }
        else { 
            /* ============[ Set output buffer for png ]============== */

            /* Overshoot of estimated size (because of compression) */
            buffer->reserve(width * height * channels);

            png_set_write_fn(png_ptr, buffer, 
                [](png_structp  png_ptr, png_bytep data, png_size_t length) {
                    std::vector<uint8_t>* buffer = (std::vector<uint8_t>*) png_get_io_ptr(png_ptr);
                    buffer->insert(buffer->end(), data, data + length);
                }, 
                [](png_structp png_ptr) {
                    /* Flush callback. */
                });
        }

        int color_type;
        if(channels < 3) {
            /* If there are two channels, just show the first one. */
            color_type = PNG_COLOR_TYPE_GRAY;
            channels = 1;
        }
        else {
            /* If there are mroe than three, just show the first three as RGB. */
            color_type = PNG_COLOR_TYPE_RGB;
            channels = 3;
        }

        /* ============[ Set main png info ]============== */
        png_set_IHDR(png_ptr, info_ptr, 
            width, 
            height,
            8, /* output bit depth */
            color_type, 
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT);

        /* ============[ Set output color space ]============== */
        png_set_sRGB(png_ptr, info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);

        /* ============[ Set progress updater ]============== */
        png_set_write_status_fn(png_ptr, 
            [](png_structp png_ptr, png_uint_32 row, int pass) {
                /* custom progress updater goes here */
            });

        int row_flush_freq = 10;
        uint32_t compression_buffer_size = width * channels * row_flush_freq;

        /* ============[ Minimum compression for speed ]============== */
        png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_NO_FILTERS);
        png_set_compression_level(png_ptr, Z_BEST_SPEED);
        /* (Nikon test raws) Runs better on my windows: */
        png_set_compression_buffer_size(png_ptr, compression_buffer_size);
        /* (Nikon test raws) Runs better on my mac: */
        /*png_set_compression_buffer_size(png_ptr, 0x20000);*/

        /* ============[ Flush data to file every ten rows ]============== */
        png_set_flush(png_ptr, row_flush_freq);

        /* ============[ Write file header ]============== */
        png_write_info(png_ptr, info_ptr);

        /* ============[ Write each row ]============== */
        uint32_t size_of_row = width * channels * 1; /* 8 bit (1 byte) */
        uint32_t row_index;
        png_bytep output_row = new png_byte[size_of_row];


        uint32_t ch, x, y, i;
        for( y = 0; y < height; y++) {
            
            /* Start next row. */
            row_index = 0;

            /* Copy/convert bitmap row to png output row. */
            for( x = 0; x < width; x++) {
                for( ch = 0; ch < channels; ch++) {
                    i = im->getIndex(y, x, ch);
                    /* Fast, lossy conversion to 8 bit. */
                    output_row[row_index++] = png_byte(bitmap[i]);
                }
            }

            /* Write row. */
            png_write_row(png_ptr, output_row);
            
        }
        delete[] output_row;


        /* ============[ End png writing ]============== */
        png_write_end(png_ptr, info_ptr);


        /* ============[ Cleanup ]============== */
        png_destroy_write_struct(&png_ptr, &info_ptr);

        if(write_to_file)
		    fclose(output_file);
	}
}
