#include "image_processing/results/colorimetry_formater.hpp"
#include <opencv2/opencv.hpp>

void ColorimetryFormater::write_format(std::ostream &output_stream, CalibrationResults *results){
    
    #define DELIM ","
    cv::Mat deltaE_values = results->get_matrix(CM_DLETA_E_VALUES);
    cv::Mat L_ref = results->get_matrix(L_REF);
    cv::Mat a_ref = results->get_matrix(a_REF);
    cv::Mat b_ref = results->get_matrix(b_REF);
    cv::Mat L_camera = results->get_matrix(L_CAMERA);
    cv::Mat a_camera = results->get_matrix(a_CAMERA);
    cv::Mat b_camera = results->get_matrix(b_CAMERA);

    cv::Mat xyz_ref = results->get_matrix(CM_XYZ_REF);
    cv::Mat xyz_camera = results->get_matrix(CM_XYZ);

    cv::Mat camera_sigs = results->get_matrix(CM_CAMERA_SIGS);


    // cv::Mat delE_Lab[] = {
    //     results->get_matrix(CM_DLETA_E_VALUES),
    //     results->get_matrix(L_REF),
    //     results->get_matrix(a_REF),
    //     results->get_matrix(b_REF),
    //     results->get_matrix(L_CAMERA),
    //     results->get_matrix(a_CAMERA),
    //     results->get_matrix(b_CAMERA)
    // };

    // cv::Mat xyz[] = {
    //     results->get_matrix(CM_XYZ_REF),
    //     results->get_matrix(CM_XYZ)
    // };

    // cv::Mat ma

    // Write Header
    this->write_header(output_stream, DELIM); 

    // All matracies should have values for the same number of patches.
    // Although some of them are in different formats and we will have to access them differently
    int col_count = deltaE_values.cols;
    int row_count = deltaE_values.rows;
    // int delE_Lab_size = sizeof(delE_Lab)/sizeof(delE_Lab[0]);

    for(int col = 0; col < col_count; col++){
        for(int row = 0; row < row_count; row++){
            // Patch ID TODO Replace this with ID number
            output_stream << char(col+65) << ":" << row << DELIM;
            // // DeltaE
            output_stream << deltaE_values.at<double>(row, col) << DELIM;
            // Lab*_ref
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
            // xyz_camera
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

            
            // for(int i = 0; i < delE_Lab_size; i++){
            //     cv::Mat matrix = delE_Lab[i];
            //     output_stream << matrix.at<double>(row, col) << DELIM;
            // }
            output_stream << std::endl;
        }
        
    }
    output_stream << std::endl;
    output_stream << "Mean" << DELIM << results->get_double(CM_DELTA_E_AVG) << std::endl;
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