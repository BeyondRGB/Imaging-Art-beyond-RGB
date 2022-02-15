#include "backend_process/HalfSizePreview.hpp"

unsigned int HalfSizePreview::id = 0;

HalfSizePreview::HalfSizePreview() {
    this->set_process_name("GetHalfSizePreview(" + std::to_string(HalfSizePreview::id) + ")");
    HalfSizePreview::id += 1;
}

HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {

    Json filenames = this->process_data_m->get_array("filenames");
    btrgb::HalfSizeReader* reader = new btrgb::HalfSizeReader;
    std::string fname;

    for (int i = 0; i < filenames.get_size(); i++) {
        try {
            fname = filenames.string_at(i);

            /* Open and quick post-process of RAW. Then, copy bitmap to a Mat. */
            reader->open(fname);
            cv::Mat im( reader->height(), reader->width(), CV_8UC(reader->channels()) );
            reader->copyBitmap( (uint8_t*) im.data, im.rows * im.cols * im.channels() );
            reader->release();

            /* Scale the image to have a maximum width of 2000 pixels (keep same aspect ratio). */
            cv::Mat im_scaled;
            if(im.cols > 2000) {
                double scaler = double(2000) / double(im.cols);
                cv::resize(im, im_scaled, cv::Size(), scaler, scaler, cv::INTER_AREA);
            } else {
                im_scaled = im;
            }
            im.release();

            /* Wrap the Mat as an Image object. */
            btrgb::Image imObj(fname + ".HalfSize");
            imObj.initImage(im_scaled);
            


            // Need to send image here.
            


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
