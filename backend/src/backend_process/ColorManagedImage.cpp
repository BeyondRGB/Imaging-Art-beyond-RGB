#include "backend_process/ColorManagedImage.hpp"
#include <cpptrace/cpptrace.hpp>

ColorManagedImage::~ColorManagedImage() {}

void ColorManagedImage::run() {

    std::unique_ptr<btrgb::LibTiffReader> tiff_reader(new btrgb::LibTiffReader);

    try {
        std::string prj_filename = this->process_data_m->get_string("name");
        bool target_requested =
            this->process_data_m->get_bool("target_requested");
        std::ifstream prj_file(prj_filename);

        std::string local_file = "";

        if (target_requested) {
            try {
                local_file =
                    jsoncons::json::parse(prj_file)["OutPutFiles"]["CM_target"]
                        .as<std::string>();
                std::cout << local_file << std::endl;
            } catch (const std::exception &e) {
                // CM_target may be unavailable since it was added in
                // BeyondRGB 2.0 in which case end early using return
                this->coms_obj_m->send_error(
                    "Color Managed Target image is unavailable. Check that your BTRGB file is in the same folder as the color managed image.",
                    "ColorManagedImage", cpptrace::generate_trace(), true);
                return;
            }
        } else {
            local_file = jsoncons::json::parse(prj_file)["OutPutFiles"]["CM"]
                             .as<std::string>();
        }
        std::cout << local_file << std::endl;
        std::string filename =
            prj_filename.substr(0, prj_filename.find_last_of("/\\") + 1) +
            local_file;

        if (!btrgb::Image::is_tiff(filename))
            throw std::runtime_error(
                "Missing tiff image files. Ensure that all tiff files produced "
                "by BeyondRGB are in the same directory as the BTRGB project "
                "file. Including, BeyondRGB_CM_Name.tiff, BeyondRGB_CM_target_Name.tiff"
                ", BeyondRGB_SP_Name.tiff. This may be caused be editing TIFF metadata.");

        cv::Mat im;
        tiff_reader->open(filename);
        tiff_reader->copyBitmapTo(im);

        try {
            void *profile;
            uint32_t profile_size;
            tiff_reader->getColorProfile(&profile_size, &profile);

            btrgb::ColorProfiles::convert(im, profile, profile_size,
                                          (void *)btrgb::sRGB2014_icc_data,
                                          btrgb::sRGB2014_icc_size);
        } catch (const std::exception &e) {
            this->coms_obj_m->send_error(e.what(), this->get_process_name(), cpptrace::generate_trace());
        }

        /* Wrap the Mat as an Image object. */
        btrgb::Image imObj(filename);
        imObj.initImage(im);
        imObj.setColorProfile(btrgb::ColorSpace::sRGB);

        /* Send image. */
        this->coms_obj_m->send_binary(&imObj, btrgb::FULL);

    } catch (const ParsingError &e) {
        this->coms_obj_m->send_error("Invalid ColorManagedImage JSON. The BTRGB file may be corrupt, try to re-run BeyondRGB on your image.",
                                     "ColorManagedImage",
                                     cpptrace::generate_trace());
    } catch (const std::exception &e) {
        this->coms_obj_m->send_error(e.what(), "ColorManagedImage",
                                     cpptrace::generate_trace());
    }

    tiff_reader->recycle();
}
