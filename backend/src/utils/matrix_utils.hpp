#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <opencv2/opencv.hpp>

namespace btrgb{
    namespace matrix_utils{

        cv::Mat create_matrix(int row_count, int col_count, int type_id);
        
        template <typename T>
        void insert_value(cv::Mat matrix, int row, int col, T value){
            matrix.at<T>(row, col) = value;
        }      
    } // namespace matrix_utils
} // namespace btrgb

#endif //MATRIX_UTILS_H