#include "image_processing/header/Verification.h"

void Verification::execute(CommunicationObj *comms, btrgb::ArtObject *images){
    comms->send_info("Starting Verification", "Verification");
    comms->send_progress(0, "Verification");

    try {
        this->channel_count = images->getImage(ART(1))->channels();

        // Init Color Targets
        this->target1 = images->get_target(ART(1), btrgb::TargetType::VERIFICATION_TARGET);
        this->target2 = images->get_target(ART(2), btrgb::TargetType::VERIFICATION_TARGET);

        this->verification_data = target1.get_ref_data();
        std::cout << "Initialized General Verification Data" << std::endl;
    }
    catch (btrgb::ArtObj_VerificationDataNull e){
        comms->send_error(e.what(), "Verification");
        return;
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        comms->send_error("Verification called out of order. Missing at least 1 image assignment.", "Verification");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        comms->send_error(error, "Verification");
        return;
    }

    this->verify_CM_calibration(comms, images);

    comms->send_progress(0.5, "Verification");

    this->verify_SP_calibration(comms, images);

    comms->send_progress(1, "Verification");
}

void Verification::verify_CM_calibration(CommunicationObj* comms, btrgb::ArtObject* images){
    comms->send_info("Starting CM Verification", "Verification");

    // Initialize target Info
    std::cout << "Init Target Info" << std::endl;
    ColorTarget targets[] = { this->target1, this->target2 };
    int target_count = std::size(targets);
    
    // Extract M and offsets
    std::cout << "Getting Results" << std::endl;
    CalibrationResults *cm_results = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    cv::Mat M = cm_results->get_matrix(CM_M);
    cv::Mat offests = cm_results->get_matrix(CM_OFFSETS);

    // Init Camera Sigs
    std::cout << "Init Sigs" << std::endl;
    cv::Mat camera_sigs = btrgb::calibration::build_target_avg_matrix(targets, target_count, this->channel_count);
    camera_sigs = btrgb::calibration::apply_offsets(camera_sigs, offests);
    
    // Compute xyz
    std::cout << "Comput XYZ" << std::endl;
    cv::Mat xyz = M * camera_sigs;
    
    // Compute Lab* values
    std::cout << "Fill Lab*" << std::endl;
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
    std::cout << "Computing DeltaE" << std::endl;
    cv::Mat deltaE_values = cv::Mat_<double>(row_count, col_count, CV_32FC1);
    double deltaE_sum = btrgb::calibration::compute_deltaE_sum(verification_data, xyz, &deltaE_values);
    int patch_count = row_count * col_count;
    double deltaE_avg = deltaE_sum / patch_count;
    
    // Store Verification Results
    std::cout << "Storing Results" << std::endl;
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

    // Initialize target Info
    ColorTarget targets[] = { this->target1, this->target2 };
    int target_count = std::size(targets);

    // Extract M_refl
    CalibrationResults *sp_results = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    std::cout << "Getting Results" << std::endl;
    cv::Mat M_refl = sp_results->get_matrix(SP_M_refl);

    // Init Camera Sigs
    std::cout << "Buiding Cam sigs" << std::endl;
    cv::Mat camera_sigs = btrgb::calibration::build_target_avg_matrix(targets, target_count, this->channel_count);

    // Init R_camera
    std::cout << "Init R_camera" << std::endl;
    cv::Mat R_camera = M_refl * camera_sigs;

    // Init R_reference
    cv::Mat R_reference = this->verification_data->as_matrix();

    // Compute RMSE
    std::cout << "Computing RMSE" << std::endl;
    double RMSE = 0;
    
    // N should be equivilent to 36 ie. the number of wavelengths ie. the number of rows in R_camera/R_ref 
    int N = R_camera.rows;
    for(int row = 0; row < R_reference.rows; row++){
        for(int col = 0; col < R_reference.cols; col++){
            double camera_val = R_camera.at<double>(row,col);
            double ref_data_value = R_reference.at<double>(row, col);
            RMSE += pow((camera_val - ref_data_value), 2) / N;
        }
    }
    RMSE = sqrt(RMSE);

    // Store Results:
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    verification_res->store_matrix(V_R_CAMERA, R_camera);
    verification_res->store_double(V_RMSE, RMSE);
}