#include <string>

#include <image_util/Image.hpp>
#include <image_util/BitDepthFinder.hpp>
#include <image_util/image_reader/LibRawReader.hpp>
#include <image_util/image_reader/TiffReaderOpenCV.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_processing/ImageReader.h>


ImageReader::ImageReader() : LeafComponent("Reading") {}

ImageReader::~ImageReader() {
    delete this->_reader;
}

void ImageReader::_set_strategy(reader_strategy strategy) {

    /* Don't change anything if strategy already selected.
     * Otherwise, delete old strategy if exists. */
    if(this->_reader != nullptr) {
        if(this->_current_strategy == strategy) {
            _reader->recycle();
            return;
        }
        delete this->_reader;
    }

    switch(strategy) {
        case TIFF_OpenCV:   this->_reader = new btrgb::TiffReaderOpenCV; break;
        case RAW_LibRaw:    this->_reader = new btrgb::LibRawReader; break;
        case TIFF_LibTiff:  this->_reader = new btrgb::LibTiffReader; break;
        default: throw std::logic_error("[ImageReader] Invalid strategy.");
    }

    this->_current_strategy = strategy;
}



void ImageReader::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Reading In Raw Image Data!", this->get_name());

    btrgb::BitDepthFinder util;
    std::shared_ptr<int> bit_depth(new int(-1));

    double total = images->imageCount();
    double count = 0;
    comms->send_progress(0, this->get_name());
    for(const auto& [key, im] : *images) {
        comms->send_info("Loading " + im->getName() + "...", this->get_name());

        /* Initialize image reader. */
        if(btrgb::Image::is_tiff(im->getName()))
            this->_set_strategy(TIFF_LibTiff);
        else
            this->_set_strategy(RAW_LibRaw);

        try {
            cv::Mat raw_im;

            _reader->open(im->getName());
            _reader->copyBitmapTo(raw_im);
            btrgb::exif tags = _reader->getExifData(); 
            _reader->recycle();


            if(raw_im.depth() != CV_16U)
                throw std::runtime_error(" Image must be 16 bit." );


            /* Find bit depth if image is white field #1. */
            if(key == "white1") {
        
                *bit_depth = util.get_bit_depth(
                    (uint16_t*) raw_im.data,    
                    raw_im.cols, 
                    raw_im.rows,
                    raw_im.channels()
                );

                if(*bit_depth < 0)
                    throw std::runtime_error(" Bit depth detection of 'white1' failed." );

                CalibrationResults* r = images->get_results_obj(btrgb::ResultType::GENERAL);
                r->store_string(GI_MAKE, tags.make);
                r->store_string(GI_MODEL, tags.model);
            }

            /* Convert to floating point. */
            cv::Mat float_im;
            raw_im.convertTo(float_im, CV_32F, 1.0/0xFFFF);

            /* If there are four channels, assume the 2nd & 4th channels
             * are both greens and average them. */
            cv::Mat result_im;
            if( float_im.channels() == 4 )
                this->_average_greens(float_im, result_im);
            else 
                result_im = float_im;

            /* Init btrgb::Image object. */
            im->initImage(result_im);
            im->_raw_bit_depth = bit_depth;
            im->setExifTags(tags);
            
            count++;
            comms->send_progress(count/total, this->get_name());

        }
        catch(const std::exception& e) {
            throw ImgProcessingComponent::error(std::string(e.what()) + " (" + im->getName() + ")", this->get_name());
        }


    }
    
    comms->send_progress(1, this->get_name());

}


void ImageReader::_average_greens(cv::Mat& input, cv::Mat& output) {

    input.forEach<cv::Vec4f>([](cv::Vec4f& pixel, const int* pos) -> void {
        pixel[1] = ( pixel[1] + pixel[3] ) / 2 ;
    });
    
    int from_to[] = { 0,0, 1,1, 2,2 };
    output.create(input.rows, input.cols, CV_MAKETYPE(input.depth(), 3));
    cv::mixChannels( &input, 1, &output, 1, from_to, 3);
}


