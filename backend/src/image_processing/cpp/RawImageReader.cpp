#include <string>

#include "ImageUtil/Image.hpp"

#include "../header/RawImageReader.h"

void RawImageReader::InternalRawProcessor::custom_process() {

    try {
        /* Extract RAW data into image struct. */
        raw2image();
    
        /* Do a basic bilinear interpolation. */
        pre_interpolate();
        imgdata.progress_flags |= LIBRAW_PROGRESS_PRE_INTERPOLATE;
        lin_interpolate();
        imgdata.progress_flags |= LIBRAW_PROGRESS_INTERPOLATE;

        /* Finalize image? */
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

RawImageReader::RawImageReader() {}
RawImageReader::~RawImageReader() {}

void RawImageReader::configLibRawParams() {
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

void RawImageReader::execute(CallBackFunction func, btrgb::ArtObject* images) {
    configLibRawParams();

    func("Reading In Raw Image Data!");

    int width, height, channels, bits_per_pixel, ec;
    btrgb::pixel* bitmap;    
    
    /* For each btrgb::image struct in the art object. */
    for(const auto& [key, im] : *images) {

        /* Open the file. */
        ec = this->rawReader.open_file(im->filename().c_str());
        if(ec) {
            images->deleteImage(key);
            this->rawReader.recycle();
            func("RawImageReader[rawReader.open_file]: " + std::to_string(ec));
            continue;
        }

        /* Read meta data if needed. */

        /* Unpack raw data into structures for processing. */
        ec = this->rawReader.unpack();
        if(ec) {
            this->rawReader.recycle();
            func("RawImageReader[rawReader.unpack]: " + std::to_string(ec));
            continue;
        }

        /* Run custom LibRaw processing routine. */
        try {
            this->rawReader.custom_process();
        }
        catch(LibRaw_exceptions e) {
            this->rawReader.recycle();
            func("RawImageReader[rawReader.custom_process]");
            continue;
        }

        /* Allocate memory for the processed image.
         * Bits per pixel should always return as 16 since that is what we configured up top. */
        this->rawReader.get_mem_image_format(&width, &height, &channels, &bits_per_pixel);
        func(
            "RAW image: width=" + std::to_string(width) + 
            " height=" + std::to_string(height) + 
            " channels=" + std::to_string(channels)
        );
        im->initBitmap(width, height, channels);

        /* Copy image data into bitmap. */
        ec = this->rawReader.copy_mem_image(im->bitmap(), width * channels * 2 /* 2 bytes */, 0);
        if(ec) {
            this->rawReader.recycle();
            im->recycle(); /* Reset image object to just the filename. */
            func("RawImageReader[rawReader.copy_mem_image]: " + std::to_string(ec));
            continue;
        }

        /* Reset the LibRAW object. */
        this->rawReader.recycle();

    }
}
