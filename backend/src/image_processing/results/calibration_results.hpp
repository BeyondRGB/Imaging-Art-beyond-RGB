#ifndef CALIBRATION_RESULTS_H
#define CALIBRATION_RESULTS_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "utils/csv_parser.hpp"

// #define RES_NAME "result_name"
#define DELIMITER ","
// #define RES_HEADER(name) RES_NAME << DELIMITER << name
// #define RES_ROW(row) "row" << row << DELIMITER
// #define MATRIX_INFO(rows, cols) "r_count:"rows DELIMITER "c_count:"cols
#define ROW_COUNT "row_count"
#define COL_COUNT "col_count"
#define M_TYPE "cv_type_id"
#define R_TYPE "ResultType_id"

class CalibrationResults: private CSVParser{

public:
    enum ResultMatrix{
        // ColorManagedCalibration
        DELTA_E,
        M, 
        OFFSETS,
        COLOR_PATCH_AVGS,
        // SpectralCalibration
        M_REFL
    };
    /**
	* Maps ResultsMatrix enum values to a string
	*/
	const std::string matrix_key_map[5] = {
        // ColorManagedCalibration
		"Delta_E",
		"M", 
        "Offestes", 
        "ColorPatchAvgs", 
        // SpectralCalibration
        "M_Refl"
	};

    enum ResultInt{

    };

    enum ResultDouble{

    };

    enum ResultType{
        INT, DOUBLE, MATRIX
    };
    /**
	* Maps ResultsMatrix enum values to a string
	*/
	const std::string type_key_map[3] = {
		"int", "double", "matrix"
	};

   
    CalibrationResults(){};
    CalibrationResults(std::string results_file);
    ~CalibrationResults();
    void set_result_matrix(std::string key, cv::Mat result);
    void set_result_int(std::string key, int value);
    void set_result_double(std::string key, double value);
    cv::Mat get_result_matrix(std::string key);
    int get_result_int(std::string key);
    double get_result_double(std::string key);
    void write_results(std::ostream &output_stream);

private:
    std::unordered_map<std::string, cv::Mat> result_matricies;
    std::unordered_map<std::string, int> result_ints;
    std::unordered_map<std::string, double> result_doubles;
    int cur_mat_type;

    void read_results(std::string results_file);
    std::unordered_map<std::string, int> pars_result_info(std::string info_string);
    
    void write_matrices(std::ostream &output_stream);
    void write_ints(std::ostream &output_stream);
    void write_doubls(std::ostream &output_stream);
    void write_matrix(std::ostream &output_stream, cv::Mat matrix, std::string matrix_name = "");
    void write_matrix_value(std::ostream &output_stream, cv::Mat matrix, int row, int col);
    
    void init_matrix(std::string name, std::unordered_map<std::string, int> info_map);
    cv::Mat create_matrix(int row_count, int col_count, int type_id);
    void append_row(std::string line, int row_num, cv::Mat matrix);

    void init_int(std::string name);
    void init_double(std::string name);

    void scane_for_next_result();
    void report_error(std::string error);
};

class ResultError: public std::exception{
    public:
    ResultError();
    ResultError(std::string erorr_msg){ error = erorr_msg; }
    virtual char const* what() const noexcept {return error.c_str();}

    private:
    std::string error = "Undefined Resulterror";
};

#endif // CALIBRATION_RESULTS_H