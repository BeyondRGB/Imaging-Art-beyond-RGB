#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <opencv2/opencv.hpp>

namespace btrgb{
    namespace matrix_utils{

        /**
         * @brief Create a matrix object of specified type
         * 
         * @param row_count number of rows the matrix should have
         * @param col_count number of cols the matrix should have
         * @param type_id the id that identifies the type of the matrix. 
         * This should be the same value that a matrix of the same type would give when calling <matrix>.typ()
         * @return cv::Mat 
         */
        cv::Mat create_matrix(int row_count, int col_count, int type_id);
        
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