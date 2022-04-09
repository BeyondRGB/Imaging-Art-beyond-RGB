#include "image_processing/results/colorimetry_formater.hpp"
#include <opencv2/opencv.hpp>

#include "utils/calibration_util.hpp"

void ColorimetryFormater::write_format(std::ostream &output_stream, CalibrationResults *results, ResultObjType format_type){
    
    #define DELIM ","
    // Declare Data
    cv::Mat deltaE_values;
    cv::Mat L_ref;
    cv::Mat a_ref;
    cv::Mat b_ref;
    cv::Mat L_camera;
    cv::Mat a_camera;
    cv::Mat b_camera;
    cv::Mat xyz_camera;
    cv::Mat camera_sigs;
    cv::Mat xyz_ref;
    double deltaE_mean;

    // Extract Data from Results obj
    try{
        if(format_type == ResultObjType::CALIBRATION){
            std::cout << "Colorimetry Calibration" << std::endl;
            deltaE_values = results->get_matrix(CM_DLETA_E_VALUES);
            L_ref = results->get_matrix(L_REF);
            a_ref = results->get_matrix(a_REF);
            b_ref = results->get_matrix(b_REF);
            L_camera = results->get_matrix(L_CAMERA);
            a_camera = results->get_matrix(a_CAMERA);
            b_camera = results->get_matrix(b_CAMERA);
            xyz_camera = results->get_matrix(CM_XYZ);
            camera_sigs = results->get_matrix(CM_CAMERA_SIGS);
            deltaE_mean = results->get_double(CM_DELTA_E_AVG);
        }
        else if(format_type == ResultObjType::VERIFICATION){
            std::cout << "Colorimetry Verification" << std::endl;
            deltaE_values = results->get_matrix(V_DLETA_E_VALUES);
            L_ref = results->get_matrix(V_L_REF);
            a_ref = results->get_matrix(V_a_REF);
            b_ref = results->get_matrix(V_b_REF);
            L_camera = results->get_matrix(V_L_CAMERA);
            a_camera = results->get_matrix(V_a_CAMERA);
            b_camera = results->get_matrix(V_b_CAMERA);
            xyz_camera = results->get_matrix(V_XYZ);
            camera_sigs = results->get_matrix(V_TARGET_SIGS);
            deltaE_mean = results->get_double(V_DELTA_E_AVG);
        }
        xyz_ref = results->get_matrix(CM_XYZ_REF);
    }catch(ResultError e){
        std::cout << "Got E: " << e.what() << std::endl;
        throw e;
    }

    
    btrgb::calibration::display_matrix(&camera_sigs, "Colorimetry cam sigs");


    // Write Header
    this->write_header(output_stream, DELIM); 

    // All matracies should have values for the same number of patches.
    // Although some of them are in different formats and we will have to access them differently
    int col_count = deltaE_values.cols;
    int row_count = deltaE_values.rows;

    for(int col = 0; col < col_count; col++){
        for(int row = 0; row < row_count; row++){
            // Patch ID
            output_stream << char(col+65) << ":" << row << DELIM;
            // // DeltaE
            output_stream << deltaE_values.at<double>(row, col) << DELIM;
            // // Lab*_ref
            output_stream << L_ref.at<double>(row, col) << DELIM;
            output_stream << a_ref.at<double>(row, col) << DELIM;
            output_stream << b_ref.at<double>(row, col) << DELIM;
            // Lab*_camera
            output_stream << L_camera.at<double>(row, col) << DELIM;
            output_stream << a_camera.at<double>(row, col) << DELIM;
            output_stream << b_camera.at<double>(row, col) << DELIM;
            
            // From here on out the all patch values are stored in single row so we need to calculate the colum position
            int mat_col = col + row * col_count;
            // xyz_ref
            /*x*/ output_stream << xyz_ref.at<double>(0, mat_col) << DELIM;
            /*y*/ output_stream << xyz_ref.at<double>(1, mat_col) << DELIM;
            /*z*/ output_stream << xyz_ref.at<double>(2, mat_col) << DELIM;
            // // xyz_camera
            /*x*/ output_stream << xyz_camera.at<double>(0, mat_col) << DELIM;
            /*y*/ output_stream << xyz_camera.at<double>(1, mat_col) << DELIM;
            /*z*/ output_stream << xyz_camera.at<double>(2, mat_col) << DELIM;
            // RGB_camera
            /*R_1*/ output_stream << camera_sigs.at<double>(0, mat_col) << DELIM;
            /*G_1*/ output_stream << camera_sigs.at<double>(1, mat_col) << DELIM;
            /*B_1*/ output_stream << camera_sigs.at<double>(2, mat_col) << DELIM;
            /*R_2*/ output_stream << camera_sigs.at<double>(3, mat_col) << DELIM;
            /*G_2*/ output_stream << camera_sigs.at<double>(4, mat_col) << DELIM;
            /*B_2*/ output_stream << camera_sigs.at<double>(5, mat_col);

            output_stream << std::endl;
        }
        
    }
    output_stream << std::endl;
    output_stream << "Mean" << DELIM << deltaE_mean << std::endl;
    #undef DELIM
}

void ColorimetryFormater::write_header(std::ostream &output_stream, std::string delim){
    int item_count = sizeof(this->colum_headers)/sizeof(this->colum_headers[0]);
    for(int i = 0; i < item_count; i++){
        if( i > 0){
            output_stream << delim;
        }
        output_stream << this->colum_headers[i];
    }
    output_stream << std::endl;
}