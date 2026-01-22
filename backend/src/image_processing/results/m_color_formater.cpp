#include "image_processing/results/m_color_formater.hpp"
#include <opencv2/opencv.hpp>

void MColorFormater::write_format(std::ostream &output_stream,
                                  CalibrationResults *results,
                                  ResultObjType format_type) {
    cv::Mat M;
    cv::Mat offset;
    try {
        M = results->get_matrix(CM_M);
        results->write_matrix(output_stream, M, "M_color");
    } catch (ParsingError e) {
        output_stream << e.what() << std::endl;
    }
    try {

        offset = results->get_matrix(CM_OFFSETS);
        results->write_matrix(output_stream, offset, "M_color_offset");
    } catch (ParsingError e) {
        output_stream << e.what() << std::endl;
    }
    try {
        offset = results->get_matrix(CM_CAMERA_SIGS);
        results->write_matrix(output_stream, offset, "color_patch_avgs");
    } catch (ParsingError e) {
        output_stream << e.what() << std::endl;
    }
}