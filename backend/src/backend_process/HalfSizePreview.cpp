#include <regex>
#include "backend_process/HalfSizePreview.hpp"

unsigned int HalfSizePreview::id = 0;

HalfSizePreview::HalfSizePreview() {
    this->set_process_name("GetHalfSizePreview(" + std::to_string(HalfSizePreview::id) + ")");
    HalfSizePreview::id += 1;
}

HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {

    Json filenames = this->process_data_m->get_array("names");
    btrgb::LibRawReader* reader = new btrgb::LibRawReader(btrgb::LibRawReader::PREVIEW);
    std::string fname;

    for (int i = 0; i < filenames.get_size(); i++) {
        try {
            fname = filenames.string_at(i);

            /* Open and quick post-process of RAW. Then, copy bitmap to a Mat. */
            reader->open(fname);
            cv::Mat im( reader->height(), reader->width(), CV_8UC(reader->channels()) );
            reader->copyBitmapTo( (uint8_t*) im.data, im.rows * im.cols * im.channels() );
            reader->recycle();

            /* Wrap the Mat as an Image object. */
            btrgb::Image imObj(fname + ".HalfSize");
            imObj.initImage(im);

            this->coms_obj_m->send_base64(&imObj, btrgb::PNG, btrgb::FAST);

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
    }

    delete reader;

}
