#include "image_processing/results/r_camera_fromater.hpp"

void RCameraFormater::write_format(std::ostream &output_stream, CalibrationResults *results, ResultObjType format_type){
    #define DELIM ","
    cv::Mat R_camera = results->get_matrix(SP_R_camera);
    cv::Mat R_ref = results->get_matrix(SP_R_reference);

    int row_count = results->get_int(GI_TARGET_ROWS);
    int col_count = results->get_int(GI_TARGET_COLS);

    this->wtrite_header(output_stream, row_count, col_count, DELIM);

    #undef DELIM

}

void RCameraFormater::wtrite_header(std::ostream &output_stream, int row_count, int col_count, std::string delim){
    output_stream << "Wavelength (nm)" << delim;
    for(int col = 0; col < col_count; col++){
        for(int row = 0; row < row_count; row++){
            output_stream << char(col+65) << ":" << row << delim;
        }
    }
    output_stream << std::endl;
}