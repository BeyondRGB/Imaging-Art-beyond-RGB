#include "image_processing/results/general_info_formater.hpp"

void GeneralInfoFormater::write_format(std::ostream &output_stream, CalibrationResults *results, ResultObjType format_type){
    #define GI_DELIM ','

    output_stream << "Input Images" << std::endl;
    for( int i = 0; i < IMG_NAME_COUNT; i++){
        std::string image_name = image_names[i];
        output_stream << image_name << GI_DELIM <<
            this->get_result<std::string>(image_name, results) << std::endl;
        
    }
    output_stream << GI_IMG_ROWS << GI_DELIM <<
        this->get_result<int>(GI_IMG_ROWS, results) << std::endl;
    output_stream << GI_IMG_COLS << GI_DELIM <<
        this->get_result<int>(GI_IMG_COLS, results) << std::endl;

    output_stream << "\nTarget Info" << std::endl;
    output_stream << GI_TARGET_ID << GI_DELIM <<
        this->get_result<std::string>(GI_TARGET_ID, results) << std::endl;
    output_stream <<  GI_TARGET_ROWS << GI_DELIM <<
        this->get_result<int>(GI_TARGET_ROWS, results) << std::endl;
    output_stream << GI_TARGET_COLS << GI_DELIM <<
        this->get_result<int>(GI_TARGET_COLS, results) << std::endl;
    output_stream << GI_TARGET_TOP << GI_DELIM <<
        this->get_result<double>(GI_TARGET_TOP, results) << std::endl;
    output_stream << GI_TARGET_BOTTOM << GI_DELIM <<
        this->get_result<double>(GI_TARGET_BOTTOM, results) << std::endl;
    output_stream << GI_TARGET_LEFT << GI_DELIM <<
        this->get_result<double>(GI_TARGET_LEFT, results) << std::endl;
    output_stream << GI_TARGET_RIGHT << GI_DELIM <<
        this->get_result<double>(GI_TARGET_RIGHT, results) << std::endl;
        

    output_stream << "\nGeneral Info" << std::endl;
    output_stream << GI_MAKE << GI_DELIM <<
        this->get_result<std::string>(GI_MAKE, results) << std::endl;
    output_stream << GI_MODEL << GI_DELIM <<
        this->get_result<std::string>(GI_MODEL, results) << std::endl;
    output_stream << GI_OBSERVER << GI_DELIM <<
        this->get_result<int>(GI_OBSERVER, results) << std::endl;
    output_stream << GI_ILLUMINANT << GI_DELIM <<
        this->get_result<std::string>(GI_ILLUMINANT, results) << std::endl;
    output_stream << GI_WHITE_PATCH_COORDS << GI_DELIM <<
        this->get_result<std::string>(GI_WHITE_PATCH_COORDS, results) << std::endl;
    output_stream << GI_Y << GI_DELIM <<
        this->get_result<double>(GI_Y, results) << std::endl;
    output_stream << GI_W << GI_DELIM <<
        this->get_result<double>(GI_W, results) << std::endl;
    output_stream << GI_ADVANCED_FILTERS << GI_DELIM <<
        this->get_result<std::string>(GI_ADVANCED_FILTERS, results) << std::endl;
    
    #undef GI_DELIM
}