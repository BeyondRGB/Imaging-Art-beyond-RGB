#include "image_processing/header/LibRawReader.h"


LibRawReader::LibRawReader() {}
LibRawReader::~LibRawReader() {}


void LibRawReader::configLibRawParams() {
	libraw_output_params_t* opt = &( this->_reader.imgdata.params );

    /* Do not subtract camera black level. */
    opt->user_black = 0;

    /* Linear */
    opt->gamm[0] = 1.0;
    opt->gamm[1] = 1.0;

    /* Keep as 16 bit. */
    opt->output_bps = 16;

    /* Bi-linear interpolation. */
    opt->user_qual = 0;

    /* Do not use histogram brightness/color scaling */
	opt->no_auto_bright = 1;
    opt->no_auto_scale = 1;

    /* Don't use white balance. */
    opt->use_camera_wb = 0;
    opt->use_auto_wb = 0;

    /* Don't use camera matrix if present. */
    opt->use_camera_matrix = 0;

    /* Use raw color space. */
    opt->output_color = 0;
}


void LibRawReader::read(btrgb::Image* im, bool record_bit_depth) {
    int width, height, channels, bits_per_pixel, error_code;


    /* Configure pre- & post-processing options. */
    this->configLibRawParams();


    /* Open the file. */
    error_code = this->_reader.open_file(im->filename().c_str());
    if(error_code) {
        this->_reader.recycle();
        throw RawReaderStrategy_FailedToOpenFile();
    }


    /* Unpack raw data into structures for processing. */
    error_code = this->_reader.unpack();
    if(error_code) {
        this->_reader.recycle();
        throw RawReaderStrategy_FailedToOpenFile();
    }


    error_code = this->_reader.dcraw_process();
    if(error_code) {
        this->_reader.recycle();
        throw std::runtime_error("[LibRaw] Failed to process image.");
    }


    /* Allocate memory for the processed image.
     * Bits per pixel should always return as 16 since that is what we configured up top. */
    this->_reader.get_mem_image_format(&width, &height, &channels, &bits_per_pixel);
    cv::Mat im16u(height, width, CV_16UC(channels));


    /* Copy image data into bitmap. */
    const int row_byte_size = width * channels * 2;
    error_code = this->_reader.copy_mem_image(im16u.data, row_byte_size, false);
    if(error_code) {
        this->_reader.recycle();
        throw RawReaderStrategy_FailedToOpenFile();
    }


    /* Estimate bit depth of raw image. */
    int raw_bit_depth = -1;
    if(record_bit_depth) {
        raw_bit_depth = BitDepthFinder().get_bit_depth( (uint16_t*) im16u.data, width, height, channels);
    }


    /* Convert 16U to 32F and copy to the btrgb::Image im. */
    cv::Mat im32f;
    im16u.convertTo(im32f, CV_32F, 1.0/0xFFFF);
    im->initImage(im32f);
    im->_raw_bit_depth = raw_bit_depth;


    /* Reset the LibRAW object. */
    this->_reader.recycle();
    im16u.release();

}
