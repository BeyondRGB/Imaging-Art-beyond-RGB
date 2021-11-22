#include <math.h>
#include <memory>
#include <libraw.h>
extern "C" {
	#include <tiffio.h>
}
//#include <libraw/libraw.h>

void write_tiff(ushort* im, int width, int height, int scale);
int getScaleValue(ushort* im, int width, int height, int channels);
int getScaleValueByFreq(ushort* im, int width, int height, int channels);

void process_image(char *file) {

	printf("LibRaw started.\r\n");

	/* Create an image processor. */
	LibRaw im;
	im.imgdata.params.output_bps = 16;
	im.imgdata.params.no_auto_bright = 1;
	im.imgdata.params.no_auto_scale = 1;
	im.imgdata.params.use_auto_wb = 0;
	im.imgdata.params.use_camera_wb = 0;
	im.imgdata.params.use_camera_matrix = 0;
	im.imgdata.params.fbdd_noiserd = 0;
	im.imgdata.params.use_rawspeed = 1;

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

	int scale = getScaleValueByFreq((ushort*) procIm->data, procIm->width, procIm->height, 3);

	/* Pass the processed bitmap to libtiff for writing. */
	write_tiff((ushort*) procIm->data, procIm->width, procIm->height, scale);

	im.dcraw_clear_mem(procIm);

	// Finally, let us free the image processor for work with the next image
	im.recycle();
}

int getScaleValue(ushort* im, int width, int height, int channels) {
	int scale = 1;
	uint16_t max = 0;
	
	int ch, x, y, i, ix, iy;

	for( y = 0; y < height; y++) {
		iy = y * width * channels;

		for( x = 0; x < width; x++) {
			ix = x * channels;
			
			for( ch = 0; ch < channels; ch++) {
				i = iy + ix + ch;

				if(im[i] > max) {
					max = im[i];
					printf("Max found on ch%d at (%d,%d): %hu\r\n", ch, x, y, max);
				}

			}
		}
	}

	int current_bits = 16;
	for (i = 15; i > 0; i--) {
		if (max >> i != 0)
			break;
	}
	current_bits = i + 1;

	/*	
	final pixel value = <current pixel value> X scale 
	final pixel value = <current pixel value> X (2^16 / 2^<current bit depth>) 
	final pixel value = <current pixel value> X 2^(16 - <current bit depth>) 
	*/ scale = (int) pow(2, 16 - current_bits);

	printf("Max pixel value: %hu\r\n", max);
	printf("Current bit depth: %d\r\n", current_bits);
	printf("Need to scale by: %d\r\n", scale);

	return scale;
}

int getScaleValueByFreq(ushort* im, int width, int height, int channels) {
	int bit_freq[3][16] = {0};
	
	int ch, x, y, i, ix, iy;
	ushort val;
	for( y = 0; y < height; y++) {
		iy = y * width * channels;

		for( x = 0; x < width; x++) {
			ix = x * channels;
			
			for( ch = 0; ch < channels; ch++) {
				i = iy + ix + ch;
				val = im[i];
				
				if(val) {
					for (i = 15; i > 0; i--) {
						if (val >> i != 0)
							break;
					}
					bit_freq[ch][i]++;
				}
				else {
					bit_freq[ch][0]++;
				}
			}
		}
	}

	printf("Bit frequency: \r\n     | %9s | %9s | %9s\r\n", "Red", "Green", "Blue");
	for( i=0; i<16; i++)
		printf("  %2d | %9d | %9d | %9d\r\n", i + 1, bit_freq[0][i], bit_freq[1][i], bit_freq[2][i]);

	/*	
	final pixel value = <current pixel value> X scale 
	final pixel value = <current pixel value> X (2^16 / 2^<current bit depth>) 
	final pixel value = <current pixel value> X 2^(16 - <current bit depth>) 
	*/int scale = 1;

	//printf("Max pixel value: %hu\r\n", max);
	//printf("Current bit depth: %d\r\n", current_bits);
	printf("Need to scale by: %d\r\n", scale);

	return scale;
}

void write_tiff(ushort* im, int width, int height, int scale) {
	int samples_per_pixel = 3; // Number of channels

	printf("Width: %d\r\nHeight: %d\r\n", width, height);

	TIFF* img_out = TIFFOpen("output.tiff", "w");

	TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);

	/* Set the number of channels. */
	TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);

	/* We'll do 16 bits, dcraw_process() defaults to 8 bits. */
	TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 16);
	
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
	ushort* sample_row = (ushort *) _TIFFmalloc(scanline_size);
	printf("Scanline Size: %lld\r\n", scanline_size);

	int col, x, y, i;
	for( y = 0; y < height; y++) {
		col = 0;
		for( x = 0; x < width; x++) {
			i = (y * width * samples_per_pixel) + (x * samples_per_pixel);
			/* R */ sample_row[col++] = im[i + 0] * scale;
			/* G */ sample_row[col++] = im[i + 1] * scale;
			/* B */ sample_row[col++] = im[i + 2] * scale;
		}
		if( y % 100 == 0 )
			printf("Progress: %3d%%\r", (y * 100) / height);
		if (TIFFWriteScanline(img_out, sample_row, y, 0) < 0)
    		break;

	}
	printf("Progress: 100%%\r\n");

	TIFFClose(img_out);
	if (sample_row)
    	_TIFFfree(sample_row);

	printf("TIFF finished.\r\n");

}

int main(int argc, char* argv[]) {
	printf("Main started\r\n");

	const char* im1;
	if(argc > 1)
		im1 = argv[1];
	else
		im1 = "nikon_targets_2.NEF";

	process_image((char*)im1);
	return 0;
}
