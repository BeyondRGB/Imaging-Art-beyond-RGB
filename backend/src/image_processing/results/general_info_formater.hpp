#ifndef GENERAL_INFO_FORMATER_H
#define GENERAL_INFO_FORMATER_H

#include "image_processing/results/results_formater.hpp"

#define IMG_NAME_COUNT 8
#define GEN_INFO_DELIM

class GeneralInfoFormater : public ResultsFormater{

    private:
        const std::string image_names[IMG_NAME_COUNT] = {
            "art1",     "art2",
            "white1",   "white2",
            "dark1",    "dark2",
            "target1",  "target2"
        };

        template <typename T>
        std::string get_result(std::string key, CalibrationResults *results){
            std::string res_string;
            try{
                if(std::is_same<T, int>::value){
                    res_string = std::to_string(results->get_int(key));
                }
                if(std::is_same<T, double>::value){
                    res_string = std::to_string(results->get_double(key));
                }
                if(std::is_same<T, std::string>::value){
                    res_string = results->get_string(key);
                }

            }catch(ResultError e){
                res_string = e.what();
            }
            return res_string;
        }
        

    public:
        void write_format(std::ostream &output_stream, CalibrationResults *results) override;

};

#endif // GENERAL_INFO_FORMATER_H