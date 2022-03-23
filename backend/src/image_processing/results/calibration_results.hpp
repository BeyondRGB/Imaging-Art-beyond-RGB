#ifndef CALIBRATION_RESULTS_H
#define CALIBRATION_RESULTS_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "utils/csv_parser.hpp"
#include "utils/jsonafiable.hpp"

// #define RES_NAME "result_name"
#define DELIMITER ","
// #define RES_HEADER(name) RES_NAME << DELIMITER << name
// #define RES_ROW(row) "row" << row << DELIMITER
// #define MATRIX_INFO(rows, cols) "r_count:"rows DELIMITER "c_count:"cols
#define ROW_COUNT "row_count"
#define COL_COUNT "col_count"
#define M_TYPE "cv_type_id"
#define R_TYPE "ResultType_id"

// CM Calibration Keys
#define CM_DELTA_E_AVG     "Average DeltaE"
#define CM_M               "CM Calibration M"
#define CM_OFFSETS         "CM Calibration Offsets"
#define CM_DLETA_E_VALUES  "CM Calibration DeltaE"

class CalibrationResults: public Jsonafiable, private CSVParser{

public:
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
    /**
     * @brief Construct a new Calibration Results object
     * This will read in given file and store any valid result found
     * 
     * @param results_file 
     */
    // CalibrationResults(std::string results_file);
    CalibrationResults(jsoncons::json json);
    ~CalibrationResults();
    
    /**
     * @brief Soters the given matrix in results map
     * 
     * @param key the key to be used to access the matrix in the results map
     * @param result the matrix to store
     */
    void store_matrix(std::string key, cv::Mat result);

    /**
     * @brief Sotres the given value in results map
     * 
     * @param key the key to be used to access the value in the rersults map
     * @param value the value to store
     */
    void store_int(std::string key, int value);

    /**
     * @brief Stores the given value in the results map
     * 
     * @param key the key to be used to access the value in the results map
     * @param value the value to store
     */
    void store_double(std::string key, double value);

    /**
     * @brief retrive matrix from the results map
     * 
     * @param key the key identifying the matrix
     * @return cv::Mat 
     */
    cv::Mat get_matrix(std::string key);

    /**
     * @brief Retrive value from the results map
     * 
     * @param key the key identifying the value
     * @return int 
     */
    int get_int(std::string key);

    /**
     * @brief Retrive value from the results map
     * 
     * @param key the key identifying the value
     * @return double 
     */
    double get_double(std::string key);

    /**
     * @brief Write all result values currently beeing stored by this class.
     * This writes to a .csv format with a single space between each result written
     *      result format
     *          <ResultName>
     *          <ResultMetaData>
     *          <ResultValue/Values> // Note: if the result is a matrix there will be additional lines following for each row
     * 
     * This will output to what ever output stream is given. 
     * If std::cout is given it will simply write to terminal.
     * If an open std::ofstream is given it will write to that file
     * 
     * @param output_stream the std::ostream to be used for writing.
     */
    void write_results(std::ostream &output_stream);

    /**
     * @brief Identifies if the instance of the class contains any result values
     * 
     * @return true 
     * @return false 
     */
    bool contains_results();


    jsoncons::json jsonafy() override;
    void de_jsonafy(jsoncons::json json) override;
    void reconstruct_matracies(Json parser);
    void reconstruct_doubles(Json parser);
    void reconstruct_ints(Json parser);


private:
    // Maps for storing various data types
    std::unordered_map<std::string, cv::Mat> result_matricies;
    std::unordered_map<std::string, int> result_ints;
    std::unordered_map<std::string, double> result_doubles;
    int cur_mat_type;

    // /**
    //  * @brief Reads in the given file.
    //  * This expects that the given file be a .csv in the form
    //  *          <Result Name>
    //  *          <Mesult MetaData>
    //  *          <Result Value/Values>
    //  * Results are expected to be seperated by an empty line
    //  * 
    //  * This will read through the entire file and store any/all valid result entrys found.
    //  * If the reader fails to pars a result the result will not be stored and the reader will move on to the next entry
    //  * 
    //  * @param results_file the file to read
    //  */
    // void read_results(std::string results_file);

    // /**
    //  * @brief Pars ResultMetaData
    //  * Each Reslut entry is expected to have MetaData emediatly after the name of the result
    //  * ResultMetaData is expected to be in the form
    //  *      <key1>:<value1>,<key2>:<value2>, ..., <keyN>:<valueN>
    //  * 
    //  * The meta data stores things souchs as 
    //  *      value type (stored as a number that maps to the enum ResultType)
    //  *      col/row count
    //  *      matrix value type
    //  * 
    //  * At minimum the meta data must include the value type
    //  * 
    //  * @param info_string 
    //  * @return std::unordered_map<std::string, int> 
    //  */
    // std::unordered_map<std::string, int> pars_result_info(std::string info_string);
    
    /**
     * @brief Itterates over every matix stored and appends its output to the given output_stream
     * 
     * @param output_stream 
     */
    void write_matrices(std::ostream &output_stream);

    /**
     * @brief Itterates over every stored integer and appends its output to the given outpu_steam
     * 
     * @param output_stream 
     */
    void write_ints(std::ostream &output_stream);

    /**
     * @brief Itterates over every stored double and append its output the given output_stream
     * 
     * @param output_stream 
     */
    void write_doubls(std::ostream &output_stream);

    /**
     * @brief Output the given matrix.
     * If a name is given the first line will be the matrix name. 
     * If no name is given the first line will be the first row of the matix
     * 
     * Each row of the matrix is writen to its own line with each value seperated by commas
     * 
     * @param output_stream the stream to write to
     * @param matrix the matrix to write
     * @param matrix_name name of the matrix defaults to empty string
     */
    void write_matrix(std::ostream &output_stream, cv::Mat matrix, std::string matrix_name = "");

    /**
     * @brief identifies what type of data is held in the given matrix and how to extract it.
     * Once value is retrieved it is written to the given output_stream
     * 
     * @param output_stream the stream to write to
     * @param matrix the matrix to get value from
     * @param row the row to get the value from
     * @param col the col to get the value from
     */
    void write_matrix_value(std::ostream &output_stream, cv::Mat matrix, int row, int col);
    
    // /**
    //  * @brief Initialize a new matrix and fill with values read in from file
    //  * Assuming that the matrix is successfuly read in, it is stored in results map
    //  * If any error occurs while trying to build the matrix an error is reported and the result is not stored
    //  * 
    //  * @param name the name of the matrix and the key that will be used to store it
    //  * @param info_map map containing the MetaData needed to reconstruc the matrix
    //  */
    // void init_matrix(std::string name, std::unordered_map<std::string, int> info_map);
    
    // /**
    //  * @brief Create a matrix object of specified size and type
    //  * 
    //  * @param row_count number of rows the matrix should have
    //  * @param col_count number of colums the matrix should have
    //  * @param type_id the type identifyer the matrix should have
    //  * @return cv::Mat 
    //  */
    // cv::Mat create_matrix(int row_count, int col_count, int type_id);

    // /**
    //  * @brief Fills matrix colums from values found in given line on specified row
    //  * This will extrace the values from the line and convert them to the type of the matrix
    //  * 
    //  * @param line a comma seperated string of values
    //  * @param row_num the row of the matrix the line represents
    //  * @param matrix the matrix to place values in
    //  */
    // void append_row(std::string line, int row_num, cv::Mat matrix);

    // /**
    //  * @brief Initialize an integer value
    //  * 
    //  * @param name 
    //  */
    // void init_int(std::string name);

    // void init_double(std::string name);

    // void scane_for_next_result();
    // void report_error(std::string error);
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