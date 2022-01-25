#include "LibpngWriter.hpp"

namespace btrgb {
    
    LibpngWriter::LibpngWriter() {this->file_extension = ".png";}
    LibpngWriter::~LibpngWriter() {}

    static void write_row_callback(png_structp png_ptr, png_uint_32 row, int pass) {
      /* custom progress updater goes here */
    }

    void LibpngWriter::_write(image* im, std::string filename) {

        /* ============[ Open file for writing ]============== */
        FILE* output_file = fopen(filename.c_str(), "wb");
		if(output_file == NULL)
			throw Libpng_OpenFileFailed();

        /* ============[ Create png_struct ]============== */
        png_structp png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING, 
            (png_voidp) NULL /*user_error_ptr*/,
            NULL /*user_error_fn*/, 
            NULL /*user_warning_fn*/
            );
        if (!png_ptr) {
            fclose(output_file);
            throw Libpng_LibraryInitFailed();
        }

        /* ============[ Create png_info ]============== */
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            fclose(output_file);
            throw Libpng_LibraryInitFailed();
        }

        /* ============[ Set up error jumping? ]============== */
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(output_file);
            throw Libpng_LibraryInitFailed();
        }

        /* ============[ Set output file for png ]============== */
        png_init_io(png_ptr, output_file);

        /* ============[ Set main png info ]============== */
        png_set_IHDR(png_ptr, info_ptr, 
            im->width(), 
            im->height(),
            8, /* output bit depth */
            PNG_COLOR_TYPE_RGB, 
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT);

        /* ============[ Set output color space ]============== */
        png_set_sRGB(png_ptr, info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);

        /* ============[ Set progress updater ]============== */
        png_set_write_status_fn(png_ptr, write_row_callback);

        /* ============[ Minimum compression for speed ]============== */
        png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_NO_FILTERS);
        png_set_compression_level(png_ptr, Z_BEST_SPEED);
        png_set_compression_buffer_size(png_ptr, 0x10000);

        /* ============[ Flush data to file every ten rows ]============== */
        png_set_flush(png_ptr, 10);

        /* ============[ Write file header ]============== */
        png_write_info(png_ptr, info_ptr);

        /* ============[ Write each row ]============== */
        uint32_t size_of_row = im->width() * im->channels() * 1; /* 8 bit writing. */
        uint32_t row_index;
        png_bytep output_row = new png_byte[size_of_row];

        int height = im->height();
        int width = im->width();
        int channels = im->channels();
        btrgb::pixel* bitmap = im->bitmap();

        uint32_t ch, x, y, i;
        for( y = 0; y < height; y++) {
            
            /* Start next row. */
            row_index = 0;

            /* Copy/convert bitmap row to png output row. */
            for( x = 0; x < width; x++) {
                for( ch = 0; ch < channels; ch++) {
                    i = im->getIndex(y, x, ch);
                    /* Fast non-arithmetic conversion from 16 to 8 bit. */
                    output_row[row_index++] = png_byte(bitmap[i] >> 8);
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

        
		fclose(output_file);
	}
}
