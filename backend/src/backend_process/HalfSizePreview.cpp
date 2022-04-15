#include <regex>
#include "backend_process/HalfSizePreview.hpp"


HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {

    try {
        Json filenames = this->process_data_m->get_array("names");

        
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

                /* Ignore 4th channel if present. */
                if(im.channels() == 4) {
                    cv::Mat rg1bg2 = im;
                    im.release();
                    im.create(rg1bg2.rows, rg1bg2.cols, CV_MAKE_TYPE(rg1bg2.depth(), 3));
                    int from_to[] = { 0,0, 1,1, 2,2 };
                    cv::mixChannels( &rg1bg2, 1, &im, 1, from_to, 3);
                }

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
    catch(const std::exception& e) {
        this->coms_obj_m->send_error("[HalfSizePreview] Request failed.", "HalfSizePreview");
        return;
    }

}
