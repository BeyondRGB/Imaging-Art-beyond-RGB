#include "image_processing/header/LibRawFileReader.h"


LibRawFileReader::LibRawFileReader() {};
LibRawFileReader::~LibRawFileReader() {};


void LibRawFileReader::configLibRawParams() {
	this->rawReader.imgdata.params.output_bps = 16;
	this->rawReader.imgdata.params.no_auto_bright = 1;
	/*this->rawReader.imgdata.params.no_auto_scale = 1;*/
	/*this->rawReader.imgdata.params.use_auto_wb = 0;*/
	/*this->rawReader.imgdata.params.use_camera_wb = 0;*/
	this->rawReader.imgdata.params.use_camera_matrix = 0;
	/*this->rawReader.imgdata.params.fbdd_noiserd = 0;*/
	/*this->rawReader.imgdata.params.use_rawspeed = 0;*/
    this->rawReader.imgdata.params.user_qual = 0;
    this->rawReader.imgdata.params.output_color = 0;
}


void LibRawFileReader::read(btrgb::image* im) {
    configLibRawParams();

    int width, height, channels, bits_per_pixel, ec;

    /* Open the file. */
    ec = this->rawReader.open_file(im->filename().c_str());
    if(ec) {
        this->rawReader.recycle();
        throw RawFileReader_FailedToOpenFile();
    }

    /* Unpack raw data into structures for processing. */
    ec = this->rawReader.unpack();
    if(ec) {
        this->rawReader.recycle();
        throw RawFileReader_FailedToOpenFile();
    }

    /* Run custom LibRaw processing routine. */
    try {
        this->rawReader.custom_process();
    }
    catch(const LibRaw_exceptions e) {
        this->rawReader.recycle();
        throw RawFileReader_FailedToOpenFile();
    }

    /* Allocate memory for the processed image.
     * Bits per pixel should always return as 16 since that is what we configured up top. */
    this->rawReader.get_mem_image_format(&width, &height, &channels, &bits_per_pixel);
    im->initBitmap(width, height, channels);

    /* Copy image data into bitmap. */
    ec = this->rawReader.copy_mem_image(
        im->bitmap(), /* Copy to image object's bitmap. */
        width * channels * 2, /* 16 bits */
        0 /* Use RGB channel order instead of BGR. */
        );
    if(ec) {
        this->rawReader.recycle();
        throw RawFileReader_FailedToOpenFile();
    }

    /* Reset the LibRAW object. */
    this->rawReader.recycle();

}


void LibRawFileReader::InternalRawProcessor::custom_process() {
    try {
        /* Extract RAW data into image struct. */
        raw2image();
    
        /* Do a basic bilinear interpolation. */
        pre_interpolate();
        imgdata.progress_flags |= LIBRAW_PROGRESS_PRE_INTERPOLATE;
        lin_interpolate();
        imgdata.progress_flags |= LIBRAW_PROGRESS_INTERPOLATE;

        /* Convert to RGB? */
        #define LR_HISTOGRAM libraw_internal_data.output_data.histogram
        if (!LR_HISTOGRAM) {
            LR_HISTOGRAM = (int(*)[LIBRAW_HISTOGRAM_SIZE]) malloc(sizeof(*LR_HISTOGRAM) * 4);
            merror(LR_HISTOGRAM, "LibRaw::dcraw_process()");
        }
        convert_to_rgb();
        imgdata.progress_flags |= LIBRAW_PROGRESS_CONVERT_RGB;
        #undef LR_HISTOGRAM
    }
    catch(const LibRaw_exceptions e) {
        throw;
    }
}
