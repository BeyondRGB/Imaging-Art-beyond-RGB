#include "jsonafiable.hpp"




jsoncons::json Jsonafiable::make_json(std::string name, cv::Mat matrix){
    jsoncons::json json;
    int rows = matrix.rows;
    int cols = matrix.cols;
    json.insert_or_assign("name", name);
    json.insert_or_assign("rows", rows);
    json.insert_or_assign("cols", cols);
    json.insert_or_assign("mat_type", matrix.type());
    jsoncons::json matrix_array = jsoncons::json::make_array<2>(rows,cols);
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            this->write_matrix_value(matrix_array, matrix, row,col);
        }
    }
    json.insert_or_assign("data", matrix_array);
    return json;
}

jsoncons::json Jsonafiable::make_json(std::string name, int value){
    jsoncons::json json;
    json.insert_or_assign("name",name);
    json.insert_or_assign("data", value);
    return json;
}

jsoncons::json Jsonafiable::make_json(std::string name, double value){
    jsoncons::json json;
    json.insert_or_assign("name",name);
    json.insert_or_assign("data", value);
    return json;
}

void Jsonafiable::write_matrix_value(jsoncons::json &json_array, cv::Mat matrix, int row, int col){
    switch(matrix.type()){
        case CV_8U:
            json_array[row][col] =  matrix.at<uchar>(row, col);
            break;
        case CV_8S:
            json_array[row][col] =  matrix.at<schar>(row, col);
            break;
        case CV_16U:
            json_array[row][col] =  matrix.at<ushort>(row, col);
            break;
        case CV_16S:
            json_array[row][col] =  matrix.at<short>(row, col);
            break;
        case CV_32S:
            json_array[row][col] =  matrix.at<int>(row, col);
            break;
        case CV_32F:
            json_array[row][col] =  matrix.at<float>(row, col);
            break;
        case CV_64F:
        default:
            json_array[row][col] =  matrix.at<double>(row, col);
            break;
    } 
}

cv::Mat Jsonafiable::reconstruct_matrix(Json matrix_json){
    int cols = matrix_json.get_value<int>("cols");
    int rows = matrix_json.get_value<int>("rows");
    int mat_type = matrix_json.get_value<int>("mat_type");
    Json mat_values = matrix_json.get_array("data");
    cv::Mat matrix = btrgb::matrix_utils::create_matrix(rows,cols,mat_type);
    for(int row = 0; row < rows; row++){
        Json row_values = mat_values.array_at(row);
        for(int col = 0; col < cols; col++){
            int value_i;
            float value_f;
            double value_d;
            switch (mat_type){
                case CV_32S:
                    value_i = row_values.value_at<int>(col);
                    btrgb::matrix_utils::insert_value<int>(matrix, row, col, value_i);
                    break;
                case CV_32F:
                    value_f = row_values.value_at<float>(col);
                    btrgb::matrix_utils::insert_value<float>(matrix, row, col, value_f);
                    break;
                case CV_64F:
                default:
                    value_d = row_values.value_at<double>(col);
                    btrgb::matrix_utils::insert_value<double>(matrix, row, col, value_d);
                    break;
            }
        }
    }
    return matrix;
}