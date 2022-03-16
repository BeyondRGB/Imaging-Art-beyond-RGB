#include "LibTiffWriter.hpp"

namespace btrgb {
    
    LibTiffWriter::LibTiffWriter() {this->file_extension = ".tiff";}
    LibTiffWriter::~LibTiffWriter() {}

    void LibTiffWriter::_write(Image* im, std::string filename) {
        struct tiff* img_out;

        int width = im->width();
        int height = im->height();
        int channels = im->channels();

		cv::Mat im_16u;
		im->getMat().convertTo(im_16u, CV_16U, 0xFFFF);
        uint16_t* bitmap = (uint16_t*) im_16u.data;



        /* ==============[ Open tiff file for writing ]================== */

		img_out = TIFFOpen(filename.c_str(), "w");
		if(!img_out) 
            throw LibTiff_OpenFileFailed();



		/* =================[ Write tiff header tags ]================== */

		TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
		TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);

		/* Set the number of channels. */
		TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, channels);

		/* We want 16 bits. */
		TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 16);
		
		/* Image rotation. */
		TIFFSetField(img_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

		/* Write all channel data in one array (a bitmap) instead of separating the channels. */
		TIFFSetField(img_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

		/* Indicate that the image is RGB. We'll need to do grayscale, perhaps 
		* as channels, or separate images in the same file. */
		TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		//TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

		/* The written data needs to be broken up into "Strips" to make buffering easier 
		* for TIFF readers. Rows-per-strip needs to be tagged, this is the number of 
		* physical pixel rows of the image written in each strip. A "row" here is a bitmap
		* row where the length is <image width> X <number of channels>. A strip size of 8kB is 
		* recommended (I wonder if this is for computing standars/capabilities wayback 
		* when). Anyway, since the data type of the return value and estimate rows-per-strip 
		* values are uint32: TIFFDefaultStripSize will not give strips that contain a fraction 
		* of a row, and our image rows are more than 8kB, so the closest to the recommended we 
		* can do is one row per strip. */
		/*
		int rows_per_strip = TIFFDefaultStripSize(img_out, 1);
		TIFFSetField(img_out, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
		*/
		TIFFSetField(img_out, TIFFTAG_ROWSPERSTRIP, 1);



        /* =================[ Write bitmap to file ]================== */

		/* The buffer for each strip to be copied to and then written to disk. 
		* If rows-per-strip is one, scanline size should equate to the number
		* of bytes for one physical pixel row of the image, and the number of
		* strips should equal the height of the image. */
		/*
		tmsize_t scanline_size = TIFFScanlineSize(img_out);
		unsigned short* sample_row = (unsigned short *) _TIFFmalloc(scanline_size);
		if (sample_row)
			_TIFFfree(sample_row);
		*/

		uint32_t row, row_size = width * channels;
        void* row_memory_address;

		/* Write all rows to file.
         * ASSUMPTION: The "rows per strip" tiff tag is set to one. */
		for( row = 0; row < height; row++) {
			
			row_memory_address = bitmap + row * row_size;

            /* Write row to file. */
			if (TIFFWriteScanline(img_out, row_memory_address, row, 0) < 0) {
                TIFFClose(img_out);
				throw LibTiff_WriteStripFailed();
            }

		}



		/* ==============[ Close tiff file ]================== */
		TIFFClose(img_out);
    }
}
