#ifndef JSONAFIABLE_H
#define JSONAFIABLE_H

#include <jsoncons/json.hpp>
#include <opencv2/opencv.hpp>

#include "json.hpp"
#include "utils/matrix_utils.hpp"

class Jsonafiable{

    public:
        virtual jsoncons::json jsonafy() = 0;
        virtual void de_jsonafy(jsoncons::json json) = 0;

        jsoncons::json make_json(std::string name, cv::Mat matrix);
        jsoncons::json make_json(std::string name, int value);
        jsoncons::json make_json(std::string name, double value);

        template <typename T>
        jsoncons::json make_json_from_map(std::string name, std::unordered_map<std::string, T> values){
            int item_count = values.size();
            jsoncons::json json_list = jsoncons::json::make_array<1>(item_count);
            int i = 0;
            for( auto [name, value] : values ){
                jsoncons::json value_json = this->make_json(name, value);
                json_list[i++] = value_json;   
            }
            return json_list;
        }

    private:
        void write_matrix_value(jsoncons::json &json_array, cv::Mat matrix, int row, int col);
        
    protected:
        cv::Mat reconstruct_matrix(Json matrix_json);

        



    


};

#endif // JSONAFIABLE_H


