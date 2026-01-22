#include <utils/jsonafiable.hpp>

jsoncons::json Jsonafiable::json_from_file(std::string file_path) {

    std::ifstream file_stream;
    file_stream.open(file_path);
    if (!file_stream.good()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    jsoncons::json_decoder<jsoncons::json> decoder;
    jsoncons::json_stream_reader reader(file_stream, decoder);

    jsoncons::json json;
    while (!reader.eof()) {
        reader.read_next();
    }
    file_stream.close();
    json = decoder.get_result();
    return json;
}

jsoncons::json Jsonafiable::make_json(std::string name, cv::Mat matrix) {
    jsoncons::json json;
    int rows = matrix.rows;
    int cols = matrix.cols;
    json.insert_or_assign(NAME_KEY, name);
    json.insert_or_assign(ROW_KEY, rows);
    json.insert_or_assign(COL_KEY, cols);
    json.insert_or_assign(MAT_TYPE_KEY, matrix.type());
    jsoncons::json matrix_array = jsoncons::json::make_array<2>(rows, cols);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            this->write_matrix_value(matrix_array, matrix, row, col);
        }
    }
    json.insert_or_assign(DATA_KEY, matrix_array);

    return json;
}

jsoncons::json Jsonafiable::make_json(std::string name, int value) {
    jsoncons::json json;
    json.insert_or_assign(NAME_KEY, name);
    json.insert_or_assign(DATA_KEY, value);
    return json;
}

jsoncons::json Jsonafiable::make_json(std::string name, double value) {
    jsoncons::json json;
    json.insert_or_assign(NAME_KEY, name);
    json.insert_or_assign(DATA_KEY, value);
    return json;
}

jsoncons::json Jsonafiable::make_json(std::string name, std::string value) {
    jsoncons::json json;
    json.insert_or_assign(NAME_KEY, name);
    json.insert_or_assign(DATA_KEY, value);
    return json;
}

void Jsonafiable::write_matrix_value(jsoncons::json &json_array, cv::Mat matrix,
                                     int row, int col) {
    switch (matrix.type()) {
    case CV_8U:
        json_array[row][col] = matrix.at<uchar>(row, col);
        break;
    case CV_8S:
        json_array[row][col] = matrix.at<schar>(row, col);
        break;
    case CV_16U:
        json_array[row][col] = matrix.at<ushort>(row, col);
        break;
    case CV_16S:
        json_array[row][col] = matrix.at<short>(row, col);
        break;
    case CV_32S:
        json_array[row][col] = matrix.at<int>(row, col);
        break;
    case CV_32F:
        json_array[row][col] = matrix.at<float>(row, col);
        break;
    case CV_64F:
    default:
        json_array[row][col] = matrix.at<double>(row, col);
        break;
    }
}

cv::Mat Jsonafiable::reconstruct_matrix(Json matrix_json) {
    cv::Mat matrix;
    try {
        int cols = matrix_json.get_value<int>(COL_KEY);
        int rows = matrix_json.get_value<int>(ROW_KEY);
        int mat_type = matrix_json.get_value<int>(MAT_TYPE_KEY);
        Json mat_values = matrix_json.get_array(DATA_KEY);
        matrix = cv::Mat(rows, cols, mat_type);
        for (int row = 0; row < rows; row++) {
            Json row_values = mat_values.array_at(row);
            for (int col = 0; col < cols; col++) {
                int value_i;
                float value_f;
                double value_d;
                switch (mat_type) {
                case CV_32S:
                    value_i = row_values.value_at<int>(col);
                    btrgb::matrix_utils::insert_value<int>(matrix, row, col,
                                                           value_i);
                    break;
                case CV_32F:
                    value_f = row_values.value_at<float>(col);
                    btrgb::matrix_utils::insert_value<float>(matrix, row, col,
                                                             value_f);
                    break;
                case CV_64F:
                default:
                    value_d = row_values.value_at<double>(col);
                    btrgb::matrix_utils::insert_value<double>(matrix, row, col,
                                                              value_d);
                    break;
                }
            }
        }
    } catch (std::exception e) {
        throw std::runtime_error("Corrupt Matrix Data");
    }
    return matrix;
}