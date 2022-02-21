#include <regex>
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

            /* Wrap the Mat as an Image object. */
            btrgb::Image imObj(fname + ".HalfSize");
            imObj.initImage(im);


            // ===============[ REPLACE CODE ]==================
            //std::string* rsp = new std::string;
            //rsp->reserve( 2000 * 2000 * 3 * 1); /* w x h x channels x byte depth -- Should be smaller when compressed, also not square */
            //rsp->append(R"({"RequestID":)");
            //rsp->append("3453456");
            //rsp->append(R"(,"RequestType":"HalfSizePreview","RequestData":{"filename":")");
            //rsp->append(std::regex_replace(filenames.string_at(i), std::regex("\\\\"), "\\\\"));
            //rsp->append(R"(","dataURL": ")");
            //btrgb::base64_ptr_t b64 = imObj.toBase64OfType(btrgb::PNG, btrgb::FAST);
            //rsp->append(*b64);
            //rsp->append(R"("}})");
            //this->send_msg(*rsp);
            //delete rsp;
            // ===========[ End REPLACE CODE ]===============
            // ===========[ REPLACEMENT CODE ]===============
            this->coms_obj_m->send_base64(&imObj, btrgb::PNG, btrgb::FAST);
            // ===========[ End REPLACEMENT CODE ]===============


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
