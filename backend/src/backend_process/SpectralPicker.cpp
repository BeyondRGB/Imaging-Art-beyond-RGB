#include "backend_process/SpectralPicker.hpp"

unsigned int SpectralPicker::id = 0;

SpectralPicker::SpectralPicker() {
    this->set_process_name("SpectralPicker(" + std::to_string(SpectralPicker::id) + ")");
    SpectralPicker::id += 1;
}

SpectralPicker::~SpectralPicker() {}


void SpectralPicker::run() {

    std::unique_ptr<btrgb::LibTiffReader> tiff_reader(new btrgb::LibTiffReader);
    
    try {
        std::string prj_filename = this->process_data_m->get_string("name");
        double x_rel = this->process_data_m->get_obj("coordinates").get_number("x");
        double y_rel = this->process_data_m->get_obj("coordinates").get_number("y");
        double size_rel = this->process_data_m->get_number("size");
        std::ifstream prj_file(prj_filename);
        std::string local_file = jsoncons::json::parse(prj_file)["OutPutFiles"]["CM"].as<std::string>();
        std::string filename = prj_filename.substr(0, prj_filename.find_last_of("/\\") + 1) + local_file;

        if( ! btrgb::Image::is_tiff(filename) )
            throw std::runtime_error("Spectral image is not a tiff file");

        
        tiff_reader->open(filename);
        int rel = tiff_reader->width();
        int size = size_rel * rel;
        int x = (x_rel * rel) - (size / 2);
        int y = (y_rel * rel) - (size / 2);

        cv::Mat im = tiff_reader->getCrop(x, y, size, size);
        cv::Mat m = tiff_reader->getConversionMatrix(BTRGB_M_REFL_OPT);




    }
    catch(const ParsingError& e) {
        this->coms_obj_m->send_error("Invalid SpectralPicker JSON", "SpectralPicker");
    }
    catch(const std::exception& e) {
        this->coms_obj_m->send_error(e.what(), "SpectralPicker");
    }

    tiff_reader->recycle();
}
