#include "calibration_results.hpp"

CalibrationResults::CalibrationResults(std::string results_file){
    this->read_results(results_file);
}

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

void CalibrationResults::write_results(std::ostream &output_stream){
    if(this->contains_results()){
        this->write_matrices(output_stream);
        this->write_ints(output_stream);
        this->write_doubls(output_stream);
    }
    else{
        output_stream << "No Results Reported.";
    }
    
}

void CalibrationResults::write_matrices(std::ostream &output_stream){
    for( auto [name, matrix] : this->result_matricies ){
        // Write Name
        output_stream << name << std::endl;
        // Write MetaData
        // output_stream << R_TYPE << ":" << ResultType::MATRIX << DELIMITER << 
        //                  ROW_COUNT <<":" << matrix.rows << DELIMITER << 
        //                  COL_COUNT << ":" << matrix.cols << DELIMITER <<
        //                  M_TYPE << ":" << matrix.type() << std::endl;
        // Write Matrix Values
        this->write_matrix(output_stream, matrix);
    }
}

void CalibrationResults::write_ints(std::ostream &output_stream){
    for( auto [name, value] : this->result_ints ){
        // Write Name
        output_stream << name << std::endl;
        // Write MetaData
        // output_stream << R_TYPE << ":" << ResultType::INT << std::endl;
        // Write Value
        output_stream << value << std::endl 
            << std::endl; // New line to prep for next result
    }
}

void CalibrationResults::write_doubls(std::ostream &output_stream){
    for( auto [name, value] : this->result_doubles ){
        // Write Name
        output_stream << name << std::endl;
        // Write MetaData
        // output_stream << R_TYPE << ":" << ResultType::DOUBLE << std::endl;
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
 
    jsoncons::json matracies = this->make_json_from_map<cv::Mat>("test", this->result_matricies);
    body.insert_or_assign("matracies", matracies);

    jsoncons::json int_values = this->make_json_from_map<int>("", this->result_ints);
    body.insert_or_assign("int_values", int_values);

    jsoncons::json double_values = this->make_json_from_map<double>("", this->result_doubles);
    body.insert_or_assign("double_values", double_values);
    
    
    return body;
}

void CalibrationResults::de_jsonafy(jsoncons::json json){
    Json parser(json);
    std::cout << "De Jsonify" << std::endl;
    Json matricies = parser.get_array("matracies");
    int item_count = matricies.get_size();
    for(int i = 0; i < item_count; i++){
        Json matrix_json = matricies.obj_at(i);
        std::string name = matrix_json.get_string("name");
        cv::Mat matrix = this->reconstruct_matrix(matrix_json);
        this->result_matricies[name] = matrix;        
    }

    Json ints =  parser.get_array("int_values");
    item_count = ints.get_size();
    for(int i = 0; i < item_count; i++){
        Json int_json = ints.obj_at(i);
        std::string name = int_json.get_string("name");
        int value = int_json.get_value<int>("data");
        this->result_ints[name] = value;
    }

    Json doubles =  parser.get_array("double_values");
    item_count = doubles.get_size();
    for(int i = 0; i < item_count; i++){
        Json double_json = doubles.obj_at(i);
        std::string name = double_json.get_string("name");
        int value = double_json.get_value<double>("data");
        this->result_doubles[name] = value;
    }
}

/**
 * We expect result files are expected to be .csv files.
 * All results found in this file are expected to be seperated by an empy line and be in the form
 *          <Result Name>
 *          <Mesult MetaData>
 *          <Result Value/Values>
 */
void CalibrationResults::read_results(std::string results_file){
    if( !this->open_file(results_file) )
        throw std::runtime_error("Failed to open file: " + results_file);
    
    while(this->has_next_line()){ 
        
        // Reading Result Name
        std::string name = this->get_next_line();
        std::cout << "Reading in: " << name << std::endl;
        
        // Reading Result MetaData
        std::string result_info = this->get_next_line();
        std::unordered_map<std::string, int> info_map;
        std::string  result_type;
        try{
            info_map = this->pars_result_info(result_info);
            result_type = type_key_map[info_map[R_TYPE]];
        }catch(ResultError e){
            this->report_error(e.what());
        }

        // Reading Result Value/Values
        if(result_type == type_key_map[ResultType::MATRIX]){
            this->init_matrix(name, info_map);
        }
        else if(result_type == type_key_map[ResultType::INT]){
            this->init_int(name);
        }
        else if(result_type == type_key_map[ResultType::DOUBLE]){
            this->init_double(name);
        }

        // Set up for reading in next result
        this->scane_for_next_result();
    }
    this->close_file();
    
}

void CalibrationResults::append_row(std::string line, int row_num, cv::Mat matrix){
    int col_num = 0;
    // while(this->has_next(line)){
    for(int col = 0; col < matrix.cols; col++){
        switch(matrix.type()){
            case CV_8U:
                matrix.at<uchar>(row_num, col_num) = this->get_next<uchar>(line);
                break;
            case CV_8S:
                matrix.at<schar>(row_num, col_num) = this->get_next<schar>(line);
                break;
            case CV_16U:
                matrix.at<ushort>(row_num, col_num) = this->get_next<ushort>(line);
                break;
            case CV_16S:
                matrix.at<short>(row_num, col_num) = this->get_next<short>(line);
                break;
            case CV_32S:
                matrix.at<int>(row_num, col_num) = this->get_next<int>(line);
                break;
            case CV_32F:
                matrix.at<float>(row_num, col_num) = this->get_next<float>(line);
                break;
            case CV_64F:
            default:
                matrix.at<double>(row_num, col_num) = this->get_next<double>(line);
                break;
        }   
        col_num++;
    }
}

std::unordered_map<std::string, int> CalibrationResults::pars_result_info(std::string info_string){
    std::unordered_map<std::string, int> info_map;
    if(info_string.length() == 0)
        throw ResultError("No MetaData Found");
    try{
        while(this->has_next(info_string)){
            // Get the current info_item
            std::string item = this->get_next<std::string>(info_string);
            // Get Key from current info_item
            std::string key = this->get_next<std::string>(item, ":");
            // Get value from current info_item
            int value = this->get_next<int>(item, ":");
            // Store key/value
            info_map[key] = value; 
        }
    }catch(std::runtime_error e){
        throw ResultError("Result MetaData: invalid format");
    } 
    return info_map;
}

void CalibrationResults::init_matrix(std::string name, std::unordered_map<std::string, int> info_map){
    cv::Mat matrix;
    if(info_map.contains(ROW_COUNT) && info_map.contains(COL_COUNT) && info_map.contains(M_TYPE)){
        matrix = this->create_matrix(info_map[ROW_COUNT], info_map[COL_COUNT], info_map[M_TYPE]);
    }
    else{
        this->report_error( name + " Matrix Result Missing MetaData");
        return; 
    }
    std::string line;
    int row_num = 0;
    for(int row = 0; row < matrix.rows; row++){   
        if(this->peek().length() == 0){
            this->report_error(name + " Missing Expected Matrix Row");
            return;
        }
        std::string line = this->get_next_line();
        try{
            this->append_row(line, row_num, matrix);
        }catch(std::exception e){
            this->report_error( name + " Missing Expected Matrix Value ");
            return;
        }
        row_num++;
    }
    this->result_matricies[name] = matrix;

}

void CalibrationResults::init_int(std::string name){
    try{
        std::string line = this->get_next_line();
        int value = this->get_next<int>(line);
        this->result_ints[name] = value;
        // this->get_next_line();
    }catch(std::exception e){
        this->report_error(name + " invalid value format");
    }
}

void CalibrationResults::init_double(std::string name){
    try{
        std::string line = this->get_next_line();
        double value = this->get_next<double>(line);
        this->result_doubles[name] = value;
        // this->get_next_line();
    }catch(std::exception e){
        this->report_error(name + " invalid value format");
    }
}

cv::Mat CalibrationResults::create_matrix(int row_count, int col_count, int type_id){
    this->cur_mat_type = type_id;
    switch(this->cur_mat_type){
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

void CalibrationResults::scane_for_next_result(){
    if(!this->has_next_line())
        return;
    // Ensure we move beyond any extra data from last result
    std::string line = this->get_next_line();
    while(line.length() != 0){
        if(!this->has_next_line())
            return;
        line = this->get_next_line();
    }

    // Ensure we move passed exess white space
    std::string next_line = this->peek();
    while(next_line.length() == 0){
        if(!this->has_next_line())
            return;
        this->get_next_line();
        next_line = this->peek();
    }
}

void CalibrationResults::report_error(std::string error){
    std::cerr << "Error: " << error << std::endl;
}