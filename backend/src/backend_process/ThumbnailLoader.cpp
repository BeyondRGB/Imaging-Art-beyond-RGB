#include "backend_process/ThumbnailLoader.hpp"

ThumbnailLoader::~ThumbnailLoader() {}


void ThumbnailLoader::run() {
    try {
        Json filenames = this->process_data_m->get_array("names");

        std::unique_ptr<btrgb::LibRawThumbnail> raw_thumbnail_reader(new btrgb::LibRawThumbnail);
        std::unique_ptr<btrgb::LibTiffReader> tiff_reader(new btrgb::LibTiffReader);
        std::string fname;

        for (int i = 0; i < filenames.get_size(); i++) {
            try {

                fname = filenames.string_at(i);
                if(btrgb::Image::is_tiff(fname))
                    this->_read_tiff(tiff_reader.get(), fname);
                else
                    this->_read_raw_thumbnail(raw_thumbnail_reader.get(), fname);

            }
            catch(btrgb::ReaderFailedToOpenFile& e) {
                this->coms_obj_m->send_error("Failed to open file " + fname, "ThumbnailLoader", cpptrace::generate_trace(), btrgb::BENING);
            }
            catch(std::runtime_error& e) {
                this->coms_obj_m->send_error(e.what(), "ThumbnailLoader", cpptrace::generate_trace(), btrgb::BENING);
            }
            catch(...) {
                this->coms_obj_m->send_error("[ThumbnailLoader] Unknown error.", "ThumbnailLoader", cpptrace::generate_trace(), btrgb::BENING);
            }

            raw_thumbnail_reader->recycle();
            tiff_reader->recycle();
        }
    }
    catch(const std::exception& e) {
        this->coms_obj_m->send_error("[ThumbnailLoader] Invalid request.", "ThumbnailLoader", cpptrace::generate_trace(), btrgb::CRITICAL);
        return;
    }

}


void ThumbnailLoader::_read_raw_thumbnail(btrgb::LibRawThumbnail* reader, std::string file) {
    reader->open(file);

    if( reader->is_encoded() ) {

        std::unique_ptr<std::vector<uchar>> binary(new std::vector<uchar>);
        reader->copyBitmapTo(*binary);
        reader->recycle();

        this->coms_obj_m->send_binary(file, binary.get(), btrgb::JPEG);
    }
    else {
        cv::Mat im;
        reader->copyBitmapTo(im);
        reader->recycle();

        /* Wrap the Mat as an Image object. */
        btrgb::Image imObj(file);
        imObj.initImage(im);

        /* Send image. */
        this->coms_obj_m->send_binary(&imObj, btrgb::FAST);
    }
}

void ThumbnailLoader::_read_tiff(btrgb::LibTiffReader* reader, std::string file) {
    /* Open tiff. */
    reader->open(file);
    cv::Mat im;
    reader->copyBitmapTo( im );
    reader->recycle();

    /* Auto bit depth (just for displaying). */
    double min, max;
    cv::minMaxIdx(im, &min, &max);
    im.convertTo(im, CV_8U, 0xFF / max);

    /* Wrap the Mat as an Image object. */
    btrgb::Image imObj(file);
    imObj.initImage(im);

    /* Send image. */
    this->coms_obj_m->send_binary(&imObj, btrgb::FAST);
}
