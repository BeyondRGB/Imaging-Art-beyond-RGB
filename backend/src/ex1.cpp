#include <memory>
#include <libraw.h>
extern "C" {
	#include <tiffio.h>
}
//#include <libraw/libraw.h>

void find_maxs(LibRaw* imr);
void write_tiff(LibRaw* imr);

void process_image(char *file) {

	printf("LibRaw started.\r\n");

	// Let us create an image processor
	LibRaw imr;

	// Open the file and read the metadata
	imr.open_file(file);

	// Let us unpack the image
	imr.unpack();

	// Convert from imgdata.rawdata to imgdata.image:
	imr.raw2image();

	printf("LibRaw finished.\r\n");

	//find_maxs(&imr);
	write_tiff(&imr);

	// Finally, let us free the image processor for work with the next image
	imr.recycle();
}


void write_tiff(LibRaw* imr) {
	int width = imr->imgdata.sizes.iwidth;
	int height = imr->imgdata.sizes.iheight;

	int samples_per_pixel = 3; // Number of channels
	//int image_row_len = width * samples_per_pixel;
	//int image_row_bytes = image_row_len * 2;

	printf("Width: %d\r\nHeight: %d\r\n", width, height);

	TIFF* img_out = TIFFOpen("output.tiff", "w");

	TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
	TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 16);

	TIFFSetField(img_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	/* Write all channel data in one array instead of separating the channels. */
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
	unsigned short* sample_row = (unsigned short *) _TIFFmalloc(scanline_size);
	printf("Scanline Size: %lld\r\n", scanline_size);
	
	int col, x, y, i;
	for( y = 0; y < height; y++) {
		col = 0;
		for( x = 0; x < width; x++) {
			i = x * y;
			/* 12 bit -> 16 bit: 2^16/2^12 = 16*/
			/* Red    */ sample_row[col++] = imr->imgdata.image[i][0] * 16;
			/* Green1 */ sample_row[col++] = imr->imgdata.image[i][1] * 16;
			/* Blue   */ sample_row[col++] = imr->imgdata.image[i][2] * 16;
			/* Green2 */ //sample_row[col++] = imr->imgdata.image[i][3];
		}
		if( y % 100 == 0 )
			printf("Row% 4d:% 5d bytes\r\n", y, col * 2);
		if (TIFFWriteScanline(img_out, sample_row, y, 0) < 0)
    		break;

	}
	printf("Row% 4d:% 5d bytes\r\n", y, col * 2);

	TIFFClose(img_out);
	if (sample_row)
    	_TIFFfree(sample_row);

}

void find_maxs(LibRaw* imr) {/*
	int width = imr->imgdata.sizes.iwidth;
	int height = imr->imgdata.sizes.iheight;
	int widthr = imr->imgdata.sizes.raw_width;
	int heightr = imr->imgdata.sizes.raw_height;

	int x, y;
    unsigned long i, ir, c;

	unsigned short max = 0, min = 0xFFFF, maxr = 0, minr = 0xFFFF, p;
	
	printf("Raw:\r\n");
    for( y = 0; y < heightr; y++) {
		for( x = 0; x < widthr; x++) {
			ir = x * y;
			p = imr->imgdata.rawdata.raw_image[ir];
			if (p > maxr) maxr = p;
			else if (p < minr) minr = p;
		}
	}

	printf("Image:\r\n");
    for( y = 0; y < height; y++) {
		for( x = 0; x < width; x++) {
			i = x * y;
			for( c= 0; c < 4; c++) {
				p = imr->imgdata.image[i][c];
				if (p > max) max = p;
				else if (p < min) min = p;
			}
		}
	}

	printf("maxr: %d\r\nminr: %d\r\nmax: %d\r\nmin: %d", maxr, minr, max, min);*/
}

int main() {
	printf("Main started\r\n");
	const char* im1 = "nikon_targets_2.NEF";
	process_image((char*)im1);
	return 0;
}
