#ifndef JSONAFIABLE_H
#define JSONAFIABLE_H

#include <fstream>
#include <jsoncons/decode_json.hpp>
#include <jsoncons/json.hpp>
#include <jsoncons/json_reader.hpp>
#include <opencv2/opencv.hpp>

#include "json.hpp"
#include "utils/matrix_utils.hpp"

#define NAME_KEY "name"
#define DATA_KEY "data"
#define ROW_KEY "rows"
#define COL_KEY "cols"
#define MAT_TYPE_KEY "mat_type"

/**
 * @brief Class to be inherited by classes that can be serialized/deserialized
 * into json It is the responsiblity of the inheritor to define jsonafy and
 * de_jsonafy
 *
 */
class Jsonafiable {

  public:
    virtual jsoncons::json jsonafy() = 0;
    virtual void de_jsonafy(jsoncons::json json) = 0;

    /**
     * @brief Static function for reading in a json object from a file
     *
     * @param file_path the name and path of the file to read in
     * @return jsoncons::json json object representing the contents found in
     * file THROWS: std::runtime_error()
     */
    static jsoncons::json json_from_file(std::string file_path);

    /**
     * @brief Creates a jons object from the given name and matrix
     *
     * @param name the key used in json
     * @param matrix the value
     * @return jsoncons::json
     */
    jsoncons::json make_json(std::string name, cv::Mat matrix);

    /**
     * @brief Creates a json object from the given name and integer value
     *
     * @param name the key used in json
     * @param value the int value to store
     * @return jsoncons::json
     */
    jsoncons::json make_json(std::string name, int value);

    /**
     * @brief Creates a json object from the given name and double value
     *
     * @param name the key used in json
     * @param value the double value to store
     * @return jsoncons::json
     */
    jsoncons::json make_json(std::string name, double value);

    /**
     * @brief Creates a json object from the given name and string value
     *
     * @param name the key used in json
     * @param value the stirng value to store
     * @return jsoncons::json
     */
    jsoncons::json make_json(std::string name, std::string value);

    /**
     * @brief Generic function for creating a json object from an unordered map
     * of type<std::string, T> This will create a json list of json objects
     * containing values of type T
     *
     * @tparam T the type of values beeing stored in the map
     * @param values the map to create the json from
     * @return jsoncons::json
     */
    template <typename T>
    jsoncons::json
    make_json_from_map(std::unordered_map<std::string, T> values) {
        int item_count = values.size();
        jsoncons::json json_list = jsoncons::json::make_array<1>(item_count);
        int i = 0;
        for (auto [name, value] : values) {
            jsoncons::json value_json = this->make_json(name, value);
            json_list[i++] = value_json;
        }
        return json_list;
    }

  private:
    void write_matrix_value(jsoncons::json &json_array, cv::Mat matrix, int row,
                            int col);

  protected:
    /**
     * @brief Reconstruct a cv::Mat from the given Json
     *
     * @param matrix_json the Json containing the matrix data
     * @return cv::Mat
     */
    cv::Mat reconstruct_matrix(Json matrix_json);
};

#endif // JSONAFIABLE_H
