#include <string>

#include <utils/threading_statics/image_reader_static.hpp>
#include <image_util/ImageLoader.hpp>
#include <image_processing/ImageReader.h>
#include <image_util/BitDepthFinder.hpp>
#include <image_util/Image.hpp>
#include <image_util/image_reader/LibRawReader.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_util/image_reader/TiffReaderOpenCV.hpp>

// sets aside memory for the static mutex (cpp static members are globally defined in namespace)
std::mutex ImageLoader::comms_mutex;

ImageLoader::ImageLoader(CommunicationObj *comms, btrgb::ArtObject *images, std::string name,
                         std::string key, btrgb::Image *im, btrgb::BitDepthFinder *util,
                         std::shared_ptr<int> bit_depth, int total_images) {
    this->comms = comms;
    this->images = images; 
    this->name = name;
    this->key = key;
    this->im = im;
    this->util = util;
    this->bit_depth = bit_depth;
    this->total_images = total_images;
}

ImageLoader::~ImageLoader() { delete this->_reader; }

void ImageLoader::_set_strategy(reader_strategy strategy) {

    /* Don't change anything if strategy already selected.
     * Otherwise, delete old strategy if exists. */
    if (this->_reader != nullptr) {
        if (this->_current_strategy == strategy) {
            _reader->recycle();
            return;
        }
        delete this->_reader;
    }

    switch (strategy) {
    case TIFF_OpenCV:
        this->_reader = new btrgb::TiffReaderOpenCV;
        break;
    case RAW_LibRaw:
        this->_reader = new btrgb::LibRawReader;
        break;
    case TIFF_LibTiff:
        this->_reader = new btrgb::LibTiffReader;
        break;
    default:
        throw std::logic_error("[ImageReader] Invalid strategy.");
    }

    this->_current_strategy = strategy;
}

void ImageLoader::_average_greens(cv::Mat &input, cv::Mat &output) {

    input.forEach<cv::Vec4f>([](cv::Vec4f &pixel, const int *pos) -> void {
        pixel[1] = (pixel[1] + pixel[3]) / 2;
    });

    int from_to[] = {0, 0, 1, 1, 2, 2};
    output.create(input.rows, input.cols, CV_MAKETYPE(input.depth(), 3));
    cv::mixChannels(&input, 1, &output, 1, from_to, 3);
}

void ImageLoader::load_image() 
{
    this->comms->send_info("Loading " + this->im->getName() + "...", this->name);

    /* Initialize image reader. */
    if (btrgb::Image::is_tiff(im->getPath()))
        this->_set_strategy(TIFF_LibTiff);
    else
        this->_set_strategy(RAW_LibRaw);

    try {
        cv::Mat raw_im;

        this->_reader->open(this->im->getPath());
        this->_reader->copyBitmapTo(raw_im);
        btrgb::exif tags = this->_reader->getExifData();
        this->_reader->recycle();

        if (raw_im.depth() != CV_16U)
            throw std::runtime_error(" Image must be 16 bit.");

        /* Find bit depth if image is white field #1. */
        if (key == "white1") {

            *this->bit_depth =
                this->util->get_bit_depth((uint16_t *)raw_im.data, raw_im.cols,
                                    raw_im.rows, raw_im.channels());

            if (*this->bit_depth < 0)
                throw std::runtime_error(
                    " Bit depth detection of 'white1' failed.");

            CalibrationResults *r =
                images->get_results_obj(btrgb::ResultType::GENERAL);
            r->store_string(GI_MAKE, tags.make);
            r->store_string(GI_MODEL, tags.model);
        }

        /* Convert to floating point. */
        cv::Mat float_im;
        raw_im.convertTo(float_im, CV_32F, 1.0 / 0xFFFF);

        /* If there are four channels, assume the 2nd & 4th channels
            * are both greens and average them. */
        cv::Mat result_im;
        if (float_im.channels() == 4)
            this->_average_greens(float_im, result_im);
        else
            result_im = float_im;

        /* Init btrgb::Image object. */
        this->im->initImage(result_im);
        this->im->_raw_bit_depth = bit_depth;
        this->im->setExifTags(tags);
        btrgb::imagereader::update_reading_progress(this->comms, &comms_mutex, this->name, this->total_images);
    } catch (const std::exception &e) {
        throw ImgProcessingComponent::error(std::string(e.what()) + " (" +
                                                this->im->getName() + ")",
                                            this->name);
    }
}