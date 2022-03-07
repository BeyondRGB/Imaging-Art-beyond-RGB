#include "../header/SpectralCalibrator.h"

void SpectralCalibrator::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("SpectralCalibration");
    
    btrgb::Image* art1;
    btrgb::Image* art2;
    ColorTarget target1;
    ColorTarget target2;

    // Extract images and RefData from art object
    try {
        art1 = images->getImage(ART(1));
        art2 = images->getImage(ART(2));
        this->ref_data = images->get_refrence_data();
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        func("Error: Flatfielding called out of order. Missing at least 1 image assignment.");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        func("Error: " + error);
        return;
    }

    // Init Color Targets
    target1 = images->get_target(ART(1));
    target2 = images->get_target(ART(2));
    ColorTarget targets[] = { target1, target2 };
    int channel_count = art1->channels();
    int target_count = std::size(targets);

    // Init Matracies used in calibration
    this->color_patch_avgs = btrgb::calibration::build_target_avg_matrix(targets, target_count, channel_count);
    this->init_M_refl();

}

void SpectralCalibrator::init_M_refl(){
    std::cout << "Initializing M_refl" << std::endl;
    int row_count = this->color_patch_avgs.rows;
    int col_count = this->color_patch_avgs.cols;
    cv::Mat psudoinvers = cv::Mat_<double>(col_count, row_count, CV_32FC1);
    cv::invert(this->color_patch_avgs, psudoinvers, cv::DECOMP_SVD);
    // btrgb::calibration::display_matrix(&this->color_patch_avgs, "Patch AVGS");
    // btrgb::calibration::display_matrix(&psudoinvers, "psudoInvers");
    cv::Mat R_ref = this->ref_data->as_matrix();
    
    cv::Mat M_refl = R_ref * psudoinvers;
    btrgb::calibration::display_matrix(&M_refl, "M Reflectance");
    std::cout << "M_refl rows: " << M_refl.rows << " M_refl cols: " << M_refl.cols << std::endl;

}