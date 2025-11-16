#include <cpptrace/cpptrace.hpp>
#include "backend_process/SpectralPicker.hpp"

SpectralPicker::~SpectralPicker() {}


void SpectralPicker::run() {

    std::unique_ptr<btrgb::LibTiffReader> tiff_reader(new btrgb::LibTiffReader);
    
    try {
        std::string prj_filename = this->process_data_m->get_string("name");
        double x_rel = this->process_data_m->get_obj("coordinates").get_number("x");
        double y_rel = this->process_data_m->get_obj("coordinates").get_number("y");
        double size_rel = this->process_data_m->get_number("size");
        std::ifstream prj_file(prj_filename);
        std::string local_file = jsoncons::json::parse(prj_file)["OutPutFiles"]["SP"].as<std::string>();
        std::string filename = prj_filename.substr(0, prj_filename.find_last_of("/\\") + 1) + local_file;

        if( ! btrgb::Image::is_tiff(filename) )
            throw std::runtime_error("Spectral image is not a tiff file");

        
        tiff_reader->open(filename);
        int width = tiff_reader->width();
        int height = tiff_reader->height();
        int size = size_rel * width;
        int radius = size/2;
        int x = x_rel * width;
        int y = y_rel * width;

        if(x < 0 || x > width || y < 0 || y > height)
            throw std::runtime_error("Invalid coordinates");

        int left = (x - radius < 0 ? 0 : x - radius);
        int right = (x + radius >= width ? width - 1 : x + radius);
        int top = (y - radius < 0 ? 0 : y - radius);
        int bot = (y + radius > height ? height : y + radius);

        cv::Mat im = tiff_reader->getCrop(left, top, right - left, bot - top);
        int channels = im.channels();
        if(channels > 16) throw std::runtime_error("Image has more than 16 channels.");

        float avg[16] = {0};
        uint16_t* im_pixel;

        for (int row = 0; row < im.rows; row++) {
            for (int col = 0; col < im.cols; col++) {
                im_pixel = (uint16_t*) im.data + row * im.cols * channels + col * channels;
                for(int ch = 0; ch < channels; ch++)
                    avg[ch] += float(im_pixel[ch]);
            }
        }

        float total_and_normalizer = float(0xffff) * float(im.rows * im.cols);
        for(int ch = 0; ch < channels; ch++)
            avg[ch] /= total_and_normalizer;

        cv::Mat avg_cam_sig(channels, 1, CV_32FC1, avg);
        cv::Mat m = tiff_reader->getConversionMatrix(BTRGB_M_REFL_OPT);
        cv::Mat spectrum = m * avg_cam_sig;

        this->coms_obj_m->send_spectrum((float*)spectrum.data, spectrum.rows);

    }
    catch(const ParsingError& e) {
        this->coms_obj_m->send_error("Invalid SpectralPicker JSON", "SpectralPicker", cpptrace::generate_trace());
    }
    catch(const std::exception& e) {
        this->coms_obj_m->send_error(e.what(), "SpectralPicker", cpptrace::generate_trace());
    }

    tiff_reader->recycle();
}
