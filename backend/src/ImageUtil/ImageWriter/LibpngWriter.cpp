#include "LibpngWriter.hpp"

namespace btrgb {
    
    LibpngWriter::LibpngWriter() {this->file_extension = ".png";}
    LibpngWriter::~LibpngWriter() {}

    void LibpngWriter::_write(image* im, std::string filename) {

        FILE* output_file = fopen(filename.c_str(), "w");
		if(output_file == NULL) {
			throw Libpng_OpenFileFailed();
		}

        png_image output_png_info;
        output_png_info.version = PNG_IMAGE_VERSION;
        output_png_info.width = im->width();
        output_png_info.height = im->height();
        output_png_info.opaque = NULL;
        output_png_info.format = PNG_FORMAT_RGB;
        output_png_info.flags = 0;
        output_png_info.colormap_entries = 0;

        int byte_size_of_row = im->width() * im->channels() * sizeof(btrgb::pixel);

        int ec = png_image_write_to_stdio(
            &output_png_info,
            output_file,
            false, /* Don't convert to 8 bit. */
            im->bitmap(),
            byte_size_of_row,
            NULL /* Not using color map */
        );

        if(ec) {
            fclose(output_file);
            throw Libpng_WriteFailed();
        }

		fclose(output_file);
	}
}
