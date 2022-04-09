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

    cv::Mat matricies_pathch_formate[] = {
        results->get_matrix(CM_DLETA_E_VALUES),
        results->get_matrix(L_REF),
        results->get_matrix(a_REF),
        results->get_matrix(b_REF),
        results->get_matrix(L_CAMERA),
        results->get_matrix(a_CAMERA),
        results->get_matrix(b_CAMERA)
    };

    // cv::Mat ma

    // Write Header
    this->write_header(output_stream, DELIM); 

    int col_count = deltaE_values.cols;
    int row_count = deltaE_values.rows;
    for(int col = 0; col < col_count; col++){
        for(int row = 0; row < row_count; row++){
            // Patch ID TODO Replace this with ID number
            output_stream << row << ":" << char(col+65) << DELIM;
            // DeltaE
            output_stream << deltaE_values.at<double>(row, col) << DELIM;

            output_stream << std::endl;
        }
        
    }
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