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

        std::cout << "\n\n\nsize: " << size << std::endl;
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        std::cout << "left: " << left << std::endl;
        std::cout << "right: " << right << std::endl;
        std::cout << "top: " << top << std::endl;
        std::cout << "bottom: " << bot << std::endl;

        cv::Mat im = tiff_reader->getCrop(left, top, right - left, bot - top);
        int channels = im.channels();
        cv::Mat avg_cam_sig(6, 1, CV_32FC1);

        float* avg = (float*) avg_cam_sig.data;
        for(int z=0; z<6; z++) avg[z] = 0;
        uint16_t* im_pixel;

        for (int row = 0; row < im.rows; row++) {
            for (int col = 0; col < im.cols; col++) {
                im_pixel = (uint16_t*) im.data + row * im.cols * channels + col * channels;
                avg[R1] += float(im_pixel[R1]);
                avg[G1] += float(im_pixel[G1]);
                avg[B1] += float(im_pixel[B1]);
                avg[R2] += float(im_pixel[R2]);
                avg[G2] += float(im_pixel[G2]);
                avg[B2] += float(im_pixel[B2]);
            }
        }

        float num_pixels = float(0xffff) * float(im.rows * im.cols);
        std::cout << "num_pixels: " << num_pixels << std::endl;
        avg[R1] /= num_pixels;
        avg[G1] /= num_pixels;
        avg[B1] /= num_pixels;
        avg[R2] /= num_pixels;
        avg[G2] /= num_pixels;
        avg[B2] /= num_pixels;

        cv::Mat m = tiff_reader->getConversionMatrix(BTRGB_M_REFL_OPT);
        cv::Mat spectrum = m * avg_cam_sig;

        this->coms_obj_m->send_spectrum((float*)spectrum.data, spectrum.rows * spectrum.cols);

    }
    catch(const ParsingError& e) {
        this->coms_obj_m->send_error("Invalid SpectralPicker JSON", "SpectralPicker");
    }
    catch(const std::exception& e) {
        this->coms_obj_m->send_error(e.what(), "SpectralPicker");
    }

    tiff_reader->recycle();
}
