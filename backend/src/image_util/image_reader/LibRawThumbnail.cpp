#include <image_util/image_reader/LibRawThumbnail.hpp>

namespace btrgb {

LibRawThumbnail::LibRawThumbnail() {}
LibRawThumbnail::~LibRawThumbnail() { this->_reader.recycle(); }

void LibRawThumbnail::open(std::string filename) {
    int error_code;

    /* Open the file. */
    error_code = this->_reader.open_file(filename.c_str());
    if (error_code) {
        this->recycle();
        throw ReaderFailedToOpenFile();
    }

    /* Unpack thumbnail data. */
    error_code = this->_reader.unpack_thumb();
    if (error_code) {
        this->recycle();
        throw std::runtime_error(
            "[LibRawThumbnail] Failed to unpack thumbnail.");
    }

    /* Determine thumbnail type. */
    switch (this->_reader.imgdata.thumbnail.tformat) {

    case LIBRAW_THUMBNAIL_JPEG:
        this->_is_encoded = true;
        this->_length = this->_reader.imgdata.thumbnail.tlength;
        break;

    case LIBRAW_THUMBNAIL_BITMAP:
        this->_is_encoded = false;
        this->_width = this->_reader.imgdata.thumbnail.twidth;
        this->_channels = this->_reader.imgdata.thumbnail.tcolors;
        this->_height = this->_reader.imgdata.thumbnail.theight;
        this->_depth = 8;
        this->_length = _width * _height * _channels * (_depth / 8);
        break;

    default:
        this->recycle();
        throw std::runtime_error("[LibRawThumbnail] Unsupported thumbnail.");
    }

    this->_is_open = true;
    this->_data = this->_reader.imgdata.thumbnail.thumb;
}

void LibRawThumbnail::recycle() {
    this->_reader.recycle();
    this->_width = -1;
    this->_height = -1;
    this->_depth = -1;
    this->_channels = -1;
    this->_is_open = false;
    this->_length = -1;
    this->_data = 0;
}

size_t LibRawThumbnail::length() { return this->_length; }
bool LibRawThumbnail::is_encoded() { return this->_is_encoded; }

void LibRawThumbnail::copyBitmapTo(std::vector<uchar> &buffer) {
    if (!this->_is_open)
        throw std::runtime_error(
            "[LibRawThumnail] Out of order call, image not open.");

    uchar *data = (uchar *)this->_data;
    uint begin_index = 0, end_index = this->_length - 1;
    buffer.reserve(this->_length);
    buffer.insert(buffer.begin(), &data[begin_index], &data[end_index]);
}

void LibRawThumbnail::copyBitmapTo(void *buffer, uint32_t size) {
    if (!this->_is_open)
        throw std::runtime_error(
            "[LibRawThumnail] Out of order call, image not open.");

    memcpy(buffer, _data, _length);
}

void LibRawThumbnail::copyBitmapTo(cv::Mat &im) {
    if (!this->_is_open)
        throw std::runtime_error(
            "[LibRawThumnail] Out of order call, image not open.");

    if (this->_is_encoded) {
        cv::Mat im_in = cv::imdecode(
            cv::Mat(1, this->_length, CV_8UC1, this->_data), cv::IMREAD_COLOR);
        cv::cvtColor(im_in, im, cv::COLOR_BGR2RGB);
    } else {
        im.create(this->_height, this->_width, CV_8UC3);
        memcpy(im.data, _data, _length);
    }
}

} // namespace btrgb
