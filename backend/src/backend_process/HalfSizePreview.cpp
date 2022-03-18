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
    std::unique_ptr<btrgb::LibRawReader> raw_reader(new btrgb::LibRawReader(btrgb::LibRawReader::PREVIEW));
    std::unique_ptr<btrgb::TiffReaderOpenCV> tiff_reader(new btrgb::TiffReaderOpenCV);
    btrgb::ImageReaderStrategy* reader;
    std::string fname;

    for (int i = 0; i < filenames.get_size(); i++) {
        try {

            /* Select image reader. */
            fname = filenames.string_at(i);
            if(btrgb::Image::is_tiff(fname))
                reader = tiff_reader.get();
            else
                reader = raw_reader.get();


            /* Open image and get Mat. */
            reader->open(fname);
            cv::Mat im;
            reader->copyBitmapTo(im);
            reader->recycle();


            /* Make sure image has a bit depth of eight. */
            if(im.depth() == CV_16U) {
                double min, max;
                cv::minMaxIdx(im, &min, &max);
                im.convertTo(im, CV_8U, 0xFF / max);
            }


            /* Wrap the Mat as an Image object. */
            btrgb::Image imObj(fname + ".HalfSize");
            imObj.initImage(im);


            /* Send image. */
            this->coms_obj_m->send_base64(&imObj, btrgb::PNG, btrgb::FAST);


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
