#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <opencv2/opencv.hpp>

namespace btrgb{
    namespace matrix_utils{
        
        /**
         * @brief Genericly typed function for inserting a value of type T into a matrix of type T
         * 
         * @tparam T the type (ie. int, double, float, ...)
         * @param matrix the matrix to insert into
         * @param row the row to insert into
         * @param col the col to insert into
         * @param value the value to insert
         */
        template <typename T>
        void insert_value(cv::Mat matrix, int row, int col, T value){
            matrix.at<T>(row, col) = value;
        }      
    } // namespace matrix_utils
} // namespace btrgb

#endif //MATRIX_UTILS_H