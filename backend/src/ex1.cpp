#include <memory>
#include <libraw.h>
extern "C" {
	#include <tiffio.h>
}
//#include <libraw/libraw.h>

void find_maxs(LibRaw* imr);
void write_tiff(uchar* im, int width, int height);
void write_tiff2(LibRaw* imr);
void raw_testing(LibRaw* im);

void process_image(char *file) {

	printf("LibRaw started.\r\n");

	/* Create an image processor. */
	LibRaw im;

	/* Open the file. */
	im.open_file(file);

	/* Read meta data if needed. */

	/* Unpack raw data into structures for processing. */
	im.unpack();

	/* Call dcraw_process() with default settings. */
	int ec = 0;
	im.dcraw_process();
	libraw_processed_image_t* procIm = im.dcraw_make_mem_image(&ec);

	printf("LibRaw finished.\r\n");
	printf("procIm data size: %u\r\n", procIm->data_size);
	printf("procIm channels: %hu\r\n", procIm->colors);
	printf("procIm byte or short (8 or 16 bits): %hu\r\n", procIm->bits);

	/* Pass the processed bitmap to libtiff for writing. */
	write_tiff((uchar*) procIm->data, procIm->width, procIm->height);

	im.dcraw_clear_mem(procIm);

	// Finally, let us free the image processor for work with the next image
	im.recycle();
}

void write_tiff(uchar* im, int width, int height) {
	int samples_per_pixel = 3; // Number of channels

	printf("Width: %d\r\nHeight: %d\r\n", width, height);

	TIFF* img_out = TIFFOpen("output.tiff", "w");

	TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);

	/* Set the number of channels. */
	TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);

	/* We'll do 16 bits, dcraw_process() defaults to 8 bits. */
	TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 8);
	
	/* Image rotation. */
	TIFFSetField(img_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	/* Write all channel data in one array (a bitmap) instead of separating the channels. */
	TIFFSetField(img_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	/* Indicate that the image is RGB. We'll need to do grayscale, perhaps 
	 * as channels, or separate images in the same file. */
	TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	/* The written data needs to be broken up into "Strips" to make buffering easier 
	 * for TIFF readers. Rows-per-strip needs to be tagged, this is the number of 
	 * physical pixel rows of the image written in each strip. A strip size of 8kB is 
	 * recommended (I wonder if this is for computing standars/capabilities wayback 
	 * when). Anyway, since the data type of the return and estimate rows-per-strip 
	 * values are uint32: we cannot break our image row up (less than one), and our image 
	 * rows are more than 8kB, so the closest to the recommended we can do is one row 
	 * per strip. */
	int rows_per_strip = TIFFDefaultStripSize(img_out, 1);
	TIFFSetField(img_out, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
	printf("Rows per strip: %d\r\n", rows_per_strip);

	/* The buffer for each strip to be copied to and then written to disk. 
	 * If rows-per-strip is one, scanline size should equate to the number
	 * of bytes for one physical pixel row of the image, and the number of
	 * strips should equal the height of the image. */
	tmsize_t scanline_size = TIFFScanlineSize(img_out);
	uchar* sample_row = (uchar *) _TIFFmalloc(scanline_size);
	printf("Scanline Size: %lld\r\n", scanline_size);

	int col, x, y, i;
	for( y = 0; y < height; y++) {
		col = 0;
		for( x = 0; x < width; x++) {
			i = (y * width * samples_per_pixel) + (x * samples_per_pixel);
			/* R */ sample_row[col++] = im[i + 0];
			/* G */ sample_row[col++] = im[i + 1];
			/* B */ sample_row[col++] = im[i + 2];
		}
		if( y % 100 == 0 )
			printf("Row% 4d:% 5d bytes  |  i: %d\r\n", y, col, i);
		if (TIFFWriteScanline(img_out, sample_row, y, 0) < 0)
    		break;

	}
	printf("Row% 4d:% 5d bytes  |  i: %d\r\n", y, col, i);

	TIFFClose(img_out);
	if (sample_row)
    	_TIFFfree(sample_row);

	printf("TIFF finished.\r\n");

}

int main() {
	printf("Main started\r\n");
	const char* im1 = "nikon_targets_2.NEF";
	process_image((char*)im1);
	return 0;
}
