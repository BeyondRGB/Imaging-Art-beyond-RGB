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
        int rel = tiff_reader->width();
        int size = size_rel * rel;
        int x = (x_rel * rel) - (size / 2);
        int y = (y_rel * rel) - (size / 2);

        std::cout << "size: " << size << std::endl;
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;

        cv::Mat im = tiff_reader->getCrop(x, y, size, size);
        int channels = im.channels();
        std::cout << "rows: " << im.rows << std::endl;
        std::cout << "cols: " << im.cols << std::endl;
        std::cout << "channels: " << im.channels() << std::endl;
        cv::Mat avg_cam_sig(6, 1, CV_32FC1);

        float* avg = (float*) avg_cam_sig.data;
        for(int z=0; z<6; z++) avg[z] = 0;
        uint16_t* im_pixel;
        std::cout << "@#$%#$@ corect type: " << (im.depth() == CV_16U) << std::endl;

        for (int row = 0; row < im.rows; row++) {
            for (int col = 0; col < im.cols; col++) {
                im_pixel = (uint16_t*) im.data + row * im.cols * channels + col * channels;
                std::cout << "pixel address: " << im_pixel << std::endl;
                avg[R1] += float(im_pixel[R1]);
                avg[G1] += float(im_pixel[G1]);
                avg[B1] += float(im_pixel[B1]);
                avg[R2] += float(im_pixel[R2]);
                avg[G2] += float(im_pixel[G2]);
                avg[B2] += float(im_pixel[B2]);

                std::cout << "Current R1: " << im_pixel[R1] << std::endl;
                std::cout << "Current G1: " << im_pixel[G1] << std::endl;
                std::cout << "Current B1: " << im_pixel[B1] << std::endl;
                std::cout << "Current R2: " << im_pixel[R2] << std::endl;
                std::cout << "Current G2: " << im_pixel[G2] << std::endl;
                std::cout << "Current B2: " << im_pixel[B2] << std::endl;
            }
        }

        std::cout << "Total R1: " << avg[R1] << std::endl;
        std::cout << "Total G1: " << avg[G1] << std::endl;
        std::cout << "Total B1: " << avg[B1] << std::endl;
        std::cout << "Total R2: " << avg[R2] << std::endl;
        std::cout << "Total G2: " << avg[G2] << std::endl;
        std::cout << "Total B2: " << avg[B2] << std::endl;

        float num_pixels = float(0xffff) * float(im.rows * im.cols);
        std::cout << "num_pixels: " << num_pixels << std::endl;
        avg[R1] /= num_pixels;
        avg[G1] /= num_pixels;
        avg[B1] /= num_pixels;
        avg[R2] /= num_pixels;
        avg[G2] /= num_pixels;
        avg[B2] /= num_pixels;


        std::cout << "Avg R1: " << avg[R1] << std::endl;
        std::cout << "Avg G1: " << avg[G1] << std::endl;
        std::cout << "Avg B1: " << avg[B1] << std::endl;
        std::cout << "Avg R2: " << avg[R2] << std::endl;
        std::cout << "Avg G2: " << avg[G2] << std::endl;
        std::cout << "Avg B2: " << avg[B2] << std::endl;

        cv::Mat m = tiff_reader->getConversionMatrix(BTRGB_M_REFL_OPT);
        cv::Mat spectrum = m * avg_cam_sig;

        std::cout << "rows: " << spectrum.rows << std::endl;
        std::cout << "cols: " << spectrum.cols << std::endl;
        avg = (float*) spectrum.data;
        for(int row = 0; row < spectrum.rows; row++)
            std::cout << avg[row] << std::endl;

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
