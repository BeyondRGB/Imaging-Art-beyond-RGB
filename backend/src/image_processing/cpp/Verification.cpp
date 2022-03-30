#include "image_processing/header/Verification.h"

void Verification::execute(CommunicationObj *comms, btrgb::ArtObject *images){
    comms->send_info("Starting Verification", "Verification");
    comms->send_progress(0, "Verification");

    this->verify_CM_calibration(comms, images);

    comms->send_progress(0.5, "Verification");

    this->verify_SP_calibration(comms, images);


    comms->send_progress(1, "Verification");
}

void Verification::verify_CM_calibration(CommunicationObj* comms, btrgb::ArtObject* images){
    comms->send_info("Starting CM Verification", "Verification");

    btrgb::Image* art1;
    btrgb::Image* art2;
    ColorTarget target1;
    ColorTarget target2;
    RefData* verification_data;

    try {
        art1 = images->getImage(ART(1));
        art2 = images->getImage(ART(2));

        // Init Color Targets
        target1 = images->get_target(ART(1), btrgb::TargetType::VERIFICATION_TARGET);
        target2 = images->get_target(ART(2), btrgb::TargetType::VERIFICATION_TARGET);

        verification_data = target1.get_ref_data();
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        comms->send_error("ColorManagedCalibrator called out of order. Missing at least 1 image assignment.", "Color Managed Calibration");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        comms->send_error(error, "Color Managed Calibration");
        return;
    }
    // Extract M and offsets
    CalibrationResults *cm_results = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    cv::Mat M = cm_results->get_matrix(CM_M);
    cv::Mat offests = cm_results->get_matrix(CM_OFFSETS);

    
    ColorTarget targets[] = { target1, target2 };
    int channel_count = art1->channels();
    int target_count = std::size(targets);

    // Init Camera Sigs
    cv::Mat camera_sigs = btrgb::calibration::build_target_avg_matrix(targets, target_count, channel_count);
    camera_sigs = btrgb::calibration::apply_offsets(camera_sigs, offests);
    // Compute xyz
    cv::Mat xyz = M * camera_sigs;
    // Compute Lab* values
    cv::Mat L_camera;
    cv::Mat a_camera;
    cv::Mat b_camera;
    cv::Mat L_ref;
    cv::Mat a_ref;
    cv::Mat b_ref;
    btrgb::calibration::fill_Lab_values(&L_camera, &a_camera, &b_camera,
                          &L_ref,    &a_ref,    &b_ref,
                          xyz, verification_data);

    int row_count = verification_data->get_row_count();
    int col_count = verification_data->get_col_count();
    // DeltaE Computations
    cv::Mat deltaE_values = cv::Mat_<double>(row_count, col_count, CV_32FC1);
    double deltaE_sum = btrgb::calibration::compute_deltaE_sum(verification_data, xyz, &deltaE_values);
    int patch_count = row_count * col_count;
    double deltaE_avg = deltaE_sum / patch_count;
    // Store Verification Results
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    verification_res->store_matrix(V_XYZ, xyz);
    verification_res->store_matrix(V_L_CAMERA, L_camera);
    verification_res->store_matrix(V_a_CAMERA, a_camera);
    verification_res->store_matrix(V_b_CAMERA, b_camera);
    verification_res->store_matrix(V_DLETA_E_VALUES, deltaE_values);
    verification_res->store_double(V_DELTA_E_AVG, deltaE_avg);
    
}

void Verification::verify_SP_calibration(CommunicationObj* comms, btrgb::ArtObject* images){
    comms->send_info("Starting SP Verification", "Verification");

}