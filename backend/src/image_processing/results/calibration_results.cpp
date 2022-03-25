#include "calibration_results.hpp"

CalibrationResults::CalibrationResults(jsoncons::json json){
    this->de_jsonafy(json);
}

CalibrationResults::~CalibrationResults(){
    for( auto [name, matrix] : this->result_matricies ){
        matrix.release();
    }
}



bool CalibrationResults::contains_results(){
    bool has_matix = this->result_matricies.size() > 0;
    bool has_int = this->result_ints.size() > 0;
    bool has_double = this->result_doubles.size() > 0;
    return has_matix || has_int || has_double;
}

void CalibrationResults::store_matrix(std::string key, cv::Mat result){
    this->result_matricies[key] = result;
}

void CalibrationResults::store_int(std::string key, int value){
    this->result_ints[key] = value;
}

void CalibrationResults::store_double(std::string key, double value){
    this->result_doubles[key] = value;
}

void CalibrationResults::store_string(std::string key, std::string value){
    this->results_strings[key] = value;
}

cv::Mat CalibrationResults::get_matrix(std::string key){
    if(this->result_matricies.contains(key)){
        cv::Mat result;
        this->result_matricies[key].copyTo(result);
        return result;
    }
    throw ResultError(key + " was not found in CalibrationResults");
}

int CalibrationResults::get_int(std::string key){
    if(this->result_ints.contains(key)){
        return this->result_ints[key];
    }
    throw ResultError(key + " was not found in CalibrationResults");
}

double CalibrationResults::get_double(std::string key){
    if(this->result_doubles.contains(key)){
        return this->result_doubles[key];
    }
    throw ResultError(key + " was not found in CalibrationResults");
}

std::string CalibrationResults::get_string(std::string key){
    if(this->results_strings.contains(key)){
        return this->results_strings[key];
    }
    throw ResultError(key + " was not found in CalibrationResults");
}

void CalibrationResults::write_results(std::ostream &output_stream){
    if(this->contains_results()){
        this->write_matrices(output_stream);
        this->write_ints(output_stream);
        this->write_doubls(output_stream);
        this->write_strings(output_stream);
    }
    else{
        output_stream << "No Results Reported.";
    }
    
}

void CalibrationResults::write_matrices(std::ostream &output_stream){
    for( auto [name, matrix] : this->result_matricies ){
        // Write Name
        output_stream << name << std::endl;
        // Write Matrix Values
        this->write_matrix(output_stream, matrix);
    }
}

void CalibrationResults::write_ints(std::ostream &output_stream){
    for( auto [name, value] : this->result_ints ){
        // Write Name
        output_stream << name << std::endl;
        // Write Value
        output_stream << value << std::endl 
            << std::endl; // New line to prep for next result
    }
}

void CalibrationResults::write_doubls(std::ostream &output_stream){
    for( auto [name, value] : this->result_doubles ){
        // Write Name
        output_stream << name << std::endl;
        // Write Value
        output_stream << value << std::endl 
            << std::endl; // New line to prep for next result
    }
}

void CalibrationResults::write_strings(std::ostream &output_stream){
    for( auto [name, value] : this->results_strings){
        // Write Name
        output_stream << name << std::endl;
        // Write Value
        output_stream << value << std::endl
            << std::endl; // New line to prep for next result
    }
}

void CalibrationResults::write_matrix(std::ostream &output_stream, cv::Mat matrix, std::string matrix_name){
    if(matrix_name.length() > 0){
        output_stream << matrix_name << std::endl;
    }
    this->cur_mat_type = matrix.type();
    for(int row = 0; row < matrix.rows; row++){
        for(int col = 0; col < matrix.cols; col++){
            this->write_matrix_value(output_stream, matrix, row, col);
            if(col < matrix.cols - 1){
                output_stream << DELIMITER;
            }
        }
        // New line for next Row
        output_stream << std::endl;
    }
    output_stream << std::endl; // New line to prep for next result
}

void CalibrationResults::write_matrix_value(std::ostream &output_stream, cv::Mat matrix, int row, int col){
    switch(matrix.type()){
        case CV_8U:
            output_stream << matrix.at<uchar>(row, col);
            break;
        case CV_8S:
            output_stream << matrix.at<schar>(row, col);
            break;
        case CV_16U:
            output_stream << matrix.at<ushort>(row, col);
            break;
        case CV_16S:
            output_stream << matrix.at<short>(row, col);
            break;
        case CV_32S:
            output_stream << matrix.at<int>(row, col);
            break;
        case CV_32F:
            output_stream << matrix.at<float>(row, col);
            break;
        case CV_64F:
        default:
            output_stream << matrix.at<double>(row, col);
            break;
    }   
}

jsoncons::json CalibrationResults::jsonafy(){
    jsoncons::json body;
    // Jsonafy the result matracies
    jsoncons::json matracies = this->make_json_from_map<cv::Mat>(this->result_matricies);
    body.insert_or_assign(MATRIX_KEY, matracies);
    // Jsonafy the result ints
    jsoncons::json int_values = this->make_json_from_map<int>(this->result_ints);
    body.insert_or_assign(INT_KEY, int_values);
    // Jsonafy the result doubles
    jsoncons::json double_values = this->make_json_from_map<double>(this->result_doubles);
    body.insert_or_assign(DOUBLE_KEY, double_values);
    // Jsonafy the result strings
    jsoncons::json string_values = this->make_json_from_map<std::string>(this->results_strings);
    body.insert_or_assign(STRING_KEY, string_values);
    
    return body;
}

void CalibrationResults::de_jsonafy(jsoncons::json json){
    try{
        Json parser(json);
        this->reconstruct_matracies(parser);
        this->reconstruct_doubles(parser);
        this->reconstruct_ints(parser);
        this->reconstruct_strings(parser);
    }catch(std::exception e){
        throw std::runtime_error("Currupt Results Json");
    }
}

void CalibrationResults::reconstruct_matracies(Json parser){
    try{
        Json matricies = parser.get_array(MATRIX_KEY);
        int item_count = matricies.get_size();
        for(int i = 0; i < item_count; i++){
            try{
                Json matrix_json = matricies.obj_at(i);
                std::string name = matrix_json.get_string(NAME_KEY);
                cv::Mat matrix = this->reconstruct_matrix(matrix_json);
                this->result_matricies[name] = matrix;        
            }catch(std::exception e){
                std::cerr << e.what() << " Found" << std::endl;
            }
        }
    }catch(std::exception e){
        std::cerr << "Corrupt Matrix List: " << e.what() << std::endl; 
    }
}

void CalibrationResults::reconstruct_ints(Json parser){
    try{
        Json ints =  parser.get_array(INT_KEY);
        int item_count = ints.get_size();
        for(int i = 0; i < item_count; i++){
            try{
                Json int_json = ints.obj_at(i);
                std::string name = int_json.get_string(NAME_KEY);
                int value = int_json.get_value<int>(DATA_KEY);
                this->result_ints[name] = value;
            }catch(std::exception e){
                std::cerr << "Currupt Int Json Found" <<  e.what() << std::endl;
            }
        }
    }catch(std::exception e){
        std::cerr << "Corrupt Int List: " << e.what() << std::endl; 
    }
}

void CalibrationResults::reconstruct_doubles(Json parser){
    try{
        Json doubles =  parser.get_array(DOUBLE_KEY);
        int item_count = doubles.get_size();
        for(int i = 0; i < item_count; i++){
            try{
                Json double_json = doubles.obj_at(i);
                std::string name = double_json.get_string(NAME_KEY);
                double value = double_json.get_value<double>(DATA_KEY);
                this->result_doubles[name] = value;
            }catch(std::exception e){
                std::cerr << "Currupt Double Json Found" <<  e.what() << std::endl;
            }
        }
    }catch(std::exception e){
        std::cerr << "Corrupt double List: " << e.what() << std::endl; 
    }
}

void CalibrationResults::reconstruct_strings(Json parser){
    try{
        Json strings = parser.get_array(STRING_KEY);
        int item_count = strings.get_size();
        for(int i = 0; i < item_count; i++){
            try{
                Json string_json = strings.obj_at(i);
                std::string name = string_json.get_string(NAME_KEY);
                std::string value = string_json.get_string(DATA_KEY);
                this->results_strings[name] = value;
            }catch(std::exception e){
                std::cerr << "Currupt String Json Found" <<  e.what() << std::endl;
            }
        }
    }catch(std::exception e){
        std::cerr << "Corrupt string List: " << e.what() << std::endl; 
    }
}

