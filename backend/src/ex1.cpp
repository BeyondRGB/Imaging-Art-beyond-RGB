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
	int sampleperpixel = 3; // Number of channels

	printf("Width: %d\r\nHeight: %d\r\n", width, height);

	TIFF* img_out = TIFFOpen("output.tiff", "w");
	printf("Width: %d\r\nHeight: %d\r\n", width, height);
	TIFFClose(img_out);

	TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);
	TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 16);

	TIFFSetField(img_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(img_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	int row_l = width * sampleperpixel;
	int row_i, x, y, i;
	unsigned short* image_row = (unsigned short *) _TIFFmalloc(TIFFScanlineSize(img_out));;

	TIFFSetField(img_out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(img_out, row_l));

	for( y = 0; y < height; y++) {
		row_i = 0;
		for( x = 0; x < width; x++) {
			i = x * y;
			image_row[row_i++] = imr->imgdata.image[i][0];
			image_row[row_i++] = imr->imgdata.image[i][1];
			image_row[row_i++] = imr->imgdata.image[i][2];
			//image_row[row_i++] = imr->imgdata.image[i][3];
		}
		if (TIFFWriteScanline(img_out, image_row, y, 0) < 0)
    		break;

	}

	TIFFClose(img_out);
	if (image_row)
    	_TIFFfree(image_row);

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
