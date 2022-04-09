#include "image_processing/results/r_camera_fromater.hpp"

void RCameraFormater::write_format(std::ostream &output_stream, CalibrationResults *results, ResultObjType format_type){
    #define DELIM ","
    cv::Mat R_camera;
    cv::Mat R_ref;
    double rmse;
    int row_count;
    int col_count;

    std::cout << "R_Camera" << std::endl;

    try{
        if(format_type == ResultObjType::CALIBRATION){
            R_camera = results->get_matrix(SP_R_camera);
            R_ref = results->get_matrix(SP_R_reference);
        }
        else if(format_type == ResultObjType::VERIFICATION){
            R_camera = results->get_matrix(V_R_CAMERA);
            R_ref = results->get_matrix(V_R_reference);
            rmse = results->get_double(V_RMSE);
        }
        
        row_count = results->get_int(GI_TARGET_ROWS);
        col_count = results->get_int(GI_TARGET_COLS);
    }catch(ResultError e){
        std::cout << "Got e: " << e.what() << std::endl;
        throw e;
    }


    output_stream << "R_camera" << std::endl;
    this->write_header(output_stream, row_count, col_count, DELIM);
    this->write_matrix(output_stream, R_camera, row_count, col_count, DELIM);
    
    output_stream << "R_reference" << std::endl;
    this->write_header(output_stream, row_count, col_count, DELIM);
    this->write_matrix(output_stream, R_ref, row_count, col_count, DELIM);

    if(format_type == ResultObjType::VERIFICATION){
        output_stream << "RMSE" << DELIM << rmse;
    }

    #undef DELIM

}

void RCameraFormater::write_header(std::ostream &output_stream, int row_count, int col_count, std::string delim){
    output_stream << "Wavelength (nm)" << delim;
    for(int col = 0; col < col_count; col++){
        for(int row = 0; row < row_count; row++){
            output_stream << char(col+65) << ":" << row << delim;
        }
    }
    output_stream << std::endl;
}

void RCameraFormater::write_matrix(std::ostream &output_stream, cv::Mat matrix, int row_count, int col_count, std::string delim){
    for(int wavelen = 0; wavelen < matrix.rows; wavelen++){
        output_stream << INDEX_TO_WAVELEN(wavelen);
        for(int col = 0; col < col_count; col++){
            for(int row = 0; row < row_count; row++){
                int mat_col = col + row * col_count;
                output_stream << delim;
                output_stream << matrix.at<double>(row, mat_col);
            }
        }
        output_stream << std::endl;
    }
    output_stream << std::endl;
}