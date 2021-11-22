#include "write_tiff.hpp"

namespace tiff_util {
	static void write_tags(TIFF* img_out, int width, int height, int channels);
	static status write_image(TIFF* img_out, unsigned short* bitmap, int width, int height, int channels);
	
	/* Writes a tiff file give a data structure. */
	status write(btrgb::image* data) {
		status s = none;
		TIFF* img_out;

		/* Open file for writing. */
		img_out = TIFFOpen(data->filename, "w");
		if(!img_out) return cannot_open_for_writing;

		/* Write tiff header tags. */
		write_tags(img_out, data->width, data->height, data->channels);

		/* Write bitmap data to tiff in strips. */
		s = write_image(img_out, data->bitmap, data->width, data->height, data->channels);
		if(s != none) {
			TIFFClose(img_out);
			return s;
		}

		/* Close the tiff file. */
		TIFFClose(img_out);

		return s;
	}

	/*
	 * This function is void because the only errors TIFFSetField can have
	 * are from invalid values (all values are statically typed here) or 
	 * a tag cannot be modified after writing (all tags are set in this function
	 * which is called before writing bitmap data).
	 */
	static void write_tags(TIFF* img_out, int width, int height, int channels) {

		TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
		TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);

		/* Set the number of channels. */
		TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, channels);

		/* We'll do 16 bits, dcraw_process() defaults to 8 bits. */
		TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 16);
		
		/* Image rotation. */
		TIFFSetField(img_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

		/* Write all channel data in one array (a bitmap) instead of separating the channels. */
		TIFFSetField(img_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

		/* Indicate that the image is RGB. We'll need to do grayscale, perhaps 
		* as channels, or separate images in the same file. */
		/*
		TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		*/
		TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

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

	}

	/*
	 * Returns tiff_util::status value of "none" on success.
	 * Returns tiff_util::status value of "failed_to_write_strip" if TIFFWriteScanline returns an error.
	 */
	static status write_image(TIFF* img_out, unsigned short* bitmap, int width, int height, int channels) {
		status s = none;

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

		int row_size = width * channels;
		int row, bitmap_index;
		/* ASSUMPTION: The "rows per strip" tiff tag is set to one. */
		for( row = 0; row < height; row++) {
			bitmap_index = row * row_size;
			if (TIFFWriteScanline(img_out, bitmap + bitmap_index, row, 0) < 0)
				return failed_to_write_strip;
		}

		return s;
	}

};