#include "matrix_utils.hpp"

cv::Mat btrgb::matrix_utils::create_matrix(int row_count, int col_count, int type_id){
    switch(type_id){
        case CV_8U:
            return cv::Mat_<uchar>(row_count, col_count);
        case CV_8S:
            return cv::Mat_<schar>(row_count, col_count);
        case CV_16U:
            return cv::Mat_<ushort>(row_count, col_count);
        case CV_16S:
            return cv::Mat_<short>(row_count, col_count);
        case CV_32S:
            return cv::Mat_<int>(row_count, col_count);
        case CV_32F:
            return cv::Mat_<float>(row_count, col_count);
        case CV_64F:
        default:
            return cv::Mat_<double>(row_count, col_count);
    }
}