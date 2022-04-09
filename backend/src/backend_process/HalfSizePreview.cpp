#include <regex>
#include "backend_process/HalfSizePreview.hpp"


HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {

    Json filenames;
    try {
        filenames = this->process_data_m->get_array("names");
    }
    catch(const std::exception& e) {
        this->coms_obj_m->send_error("[HalfSizePreview] Invalid request.", "HalfSizePreview");
        return;
    }
    
    std::unique_ptr<btrgb::LibRawReader> raw_reader(new btrgb::LibRawReader(btrgb::LibRawReader::PREVIEW));
    std::unique_ptr<btrgb::LibTiffReader> tiff_reader(new btrgb::LibTiffReader);
    btrgb::ImageReaderStrategy* reader;
    std::string fname;
    bool is_tiff;

    for (int i = 0; i < filenames.get_size(); i++) {
        try {
            fname = filenames.string_at(i);
            is_tiff = btrgb::Image::is_tiff(fname);

            /* Select image reader. */
            if(is_tiff)
                reader = tiff_reader.get();
            else
                reader = raw_reader.get();


            /* Open image and get Mat. */
            reader->open(fname);
            cv::Mat im;
            reader->copyBitmapTo(im);
            reader->recycle();

            /* Make sure image is bright enough. */
            double min, max;
            cv::minMaxIdx(im, &min, &max);
            im.convertTo(im, CV_16U, 0xFFFF / max);

            /* Wrap the Mat as an Image object. */
            btrgb::Image imObj(fname + ".HalfSize");
            imObj.initImage(im);


            /* Send image. */
            this->coms_obj_m->send_binary(&imObj, btrgb::FAST);


        }
        catch(const btrgb::LibRawFileTypeUnsupported& e) {
            this->report_error(this->get_process_name(), "File type unknown, or unsupported by LibRaw.");
        }
        catch(const ParsingError& e) {
            this->report_error(this->get_process_name(), e.what());
        }
        catch(const std::runtime_error& e) {
            this->report_error(this->get_process_name(), std::string(e.what()) + " (" + fname + ")");
        }
        catch(const btrgb::FailedToEncode& e) {
            this->report_error(this->get_process_name(), std::string(e.what()) + " (" + fname + ")");
        }
        reader->recycle();
    }

}
