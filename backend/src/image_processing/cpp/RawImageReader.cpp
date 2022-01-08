#include <string>

#include "ImageUtil/Image.hpp"

#include "../header/RawImageReader.h"


RawImageReader::RawImageReader() {}
RawImageReader::~RawImageReader() {}

void RawImageReader::configPostProcParams() {
	this->rawReader.imgdata.params.output_bps = 16;
	this->rawReader.imgdata.params.no_auto_bright = 1;
	this->rawReader.imgdata.params.no_auto_scale = 1;
	this->rawReader.imgdata.params.use_auto_wb = 0;
	this->rawReader.imgdata.params.use_camera_wb = 0;
	this->rawReader.imgdata.params.use_camera_matrix = 0;
	this->rawReader.imgdata.params.fbdd_noiserd = 0;
	this->rawReader.imgdata.params.use_rawspeed = 1;
}

void RawImageReader::execute(CallBackFunction func, btrgb::ArtObject* images) {
    configPostProcParams();

    func("Reading In Raw Image Data!");

    int width, height, channels, bits_per_pixel, ec;
    btrgb::pixel* bitmap;    
    
    /* For each btrgb::image struct in the art object. */
    for(const auto& [key, im] : *images) {

        /* Open the file. */
        ec = this->rawReader.open_file(im->filename().c_str());
        if(ec) {
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

        /* Call dcraw_process(). This call is affected by 
         * parameters set in the RawImageReader constructor. */
        ec = this->rawReader.dcraw_process();
        if(ec) {
            this->rawReader.recycle();
            func("RawImageReader[rawReader.dcraw_process]: " + std::to_string(ec));
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
