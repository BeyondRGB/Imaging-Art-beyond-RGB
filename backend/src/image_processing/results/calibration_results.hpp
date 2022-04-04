#ifndef CALIBRATION_RESULTS_H
#define CALIBRATION_RESULTS_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "utils/csv_parser.hpp"
#include "utils/jsonafiable.hpp"


#define DELIMITER ","

// CM Calibration Keys
#define CM_DELTA_E_AVG      "CM DeltaE Mean"
#define CM_M                "CM Optimized M"
#define CM_OFFSETS          "CM Optimized Offsets"
#define CM_DLETA_E_VALUES   "CM DeltaE Values"
#define CM_CAMERA_SIGS      "CM PreCalibrated RGB Camera Signals"
#define CM_XYZ              "CM Calibrated XYZ Values"
#define L_CAMERA            "CM L*_camera"
#define a_CAMERA            "CM a*_camera"
#define b_CAMERA            "CM b*_camera"
#define L_REF               "CM L*_ref"
#define a_REF               "CM a*_ref"
#define b_REF               "CM b*_ref"

// Spectral Calibration Keys
#define SP_R_reference  "SP R Reference"
#define SP_R_camera     "SP Optimized R Camera"
#define SP_M_refl       "SP Optimized M refl"

// General Info Keys
#define GI_MAKE_MODEL         "Camera Make/Model"
#define GI_TARGET_ID          "Target ID"
#define GI_TARGET_ROWS        "Target Row Count"
#define GI_TARGET_COLS        "Target Col Count"
#define GI_IMG_ROWS           "Img Row Count"
#define GI_IMG_COLS           "Img Col Count"
#define GI_OBSERVER           "Standard Observer"
#define GI_ILLUMINANT         "Illuminant"
#define GI_WHITE_PATCH_COORDS "White Patch Coordinates"
#define GI_Y                  "Y white patche meas"
#define GI_W                  "W Value"

// Verification Keys
#define V_XYZ              "Verification Calibrated XYZ Values"
#define V_DLETA_E_VALUES   "Verification DeltaE Values"
#define V_DELTA_E_AVG      "Verification DeltaE Mean"
#define V_L_CAMERA         "Verification L*_camera"
#define V_a_CAMERA         "Verification a*_camera"
#define V_b_CAMERA         "Verification b*_camera"
#define V_R_CAMERA         "Verification R_camera"
#define V_RMSE             "Verification RMSE"

#define PRO_FILE              "BTRGB Results File"

// Json Value Storage Keys
#define MATRIX_KEY "matrix_values"
#define INT_KEY "integer_values"
#define DOUBLE_KEY "double_values"
#define STRING_KEY "string_values"

/**
 * @brief Class to strore results in.
 * This class can serialize itself into a json object and deserialize itself given a json object
 * This class can store result data in the forms of String,Int,Double,cv::Mat
 * 
 */
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
     * @brief Store the given value in the results map
     * 
     * @param key the key to be used to access the value in the rsults map
     * @param value the value to store
     */
    void store_string(std::string key, std::string value);

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
     * @brief Retrive value from the results map
     * 
     * @param key the key identifying the value
     * @return std::string 
     */
    std::string get_string(std::string key);

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

    /**
     * @brief Inherited from Jsonifiable
     * This serializes the class and adds all results contained in this class to a json object
     * 
     * @return jsoncons::json 
     */
    jsoncons::json jsonafy() override;

    /**
     * @brief Inherited from Jsoifiable
     * This de-serializes the class and reconstructs a previouse instance of this class from the given json
     * 
     * NOTE: if data is missing or currupt in the given json it is posible that some or all result info does not get reconstructed
     * 
     * @param json the json containing the information 
     */
    void de_jsonafy(jsoncons::json json) override;

    /**
     * @brief Reconstruct all matrix data from the given json
     * NOTE: if any data is missing or corrupt for a matix found in json that matix will be skiped
     * 
     * @param parser 
     */
    void reconstruct_matracies(Json parser);

    /**
     * @brief Reconstruct all double data from given json
     * NOTE: if any data is missing or corrup for a double found in the json that value will be skiped
     * 
     * @param parser 
     */
    void reconstruct_doubles(Json parser);

    /**
     * @brief Reconstruct all int data from given json
     * NOTE: if any data is missing or corrup for a int found in the json that value will be skiped
     * 
     * @param parser 
     */
    void reconstruct_ints(Json parser);

    /**
     * @brief Reconstruct all string data from given json
     * NOTE: if any data is missing or corrup for a string found in the json that value will be skiped
     * 
     * @param parser 
     */
    void reconstruct_strings(Json parser);


private:
    // Maps for storing various data types
    std::unordered_map<std::string, cv::Mat> result_matricies;
    std::unordered_map<std::string, int> result_ints;
    std::unordered_map<std::string, double> result_doubles;
    std::unordered_map<std::string, std::string> results_strings;
    int cur_mat_type;
  
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

    void write_strings(std::ostream &output_stram);

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