#include "image_processing/header/ResultsProcessor.h"

void ResultsProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images){
    comms->send_info("","ResultsProcessor");
    comms->send_progress(0,"ResultsProcessor");

    this->output_dir = images->get_output_dir();
    this->ts_id = btrgb::get_time_stamp();

    // Generate the file names to be used for output
    this->CM_f_name = this->build_output_name("CM");
    this->SP_f_name = this->build_output_name("SP");
    this->CalibRes_f_name = this->build_output_name("Calibration", "csv");
    this->VerRes_f_name = this->build_output_name("Verification", "csv");
    this->GI_f_name = this->build_output_name("GeneralInfo", "txt");
    this->Pro_f_name = this->build_output_name("","btrgb");
    
    // Output Results
    this->output_user_results(images);  
    this->output_btrgb_results(images);

    comms->send_progress(0.2,"ResultsProcessor");

    // Output Images 
    this->output_images(images);

    
    // Store PRO_file so we can access it from Pipeline
    images->get_results_obj(btrgb::ResultType::GENERAL)->store_string(PRO_FILE, this->output_dir + this->Pro_f_name);
    
    comms->send_progress(1,"ResultsProcessor");
}

void ResultsProcessor::output_images(btrgb::ArtObject* images){
    try{
    // Write CM Calibrated Image
    images->outputImageAs(btrgb::TIFF, CM_IMAGE_KEY, this->CM_f_name);
    }catch(std::exception e){
        std::cerr << "Failed to write CM_Image: " << e.what() << std::endl; 
    }

    try{
    // Write Spectral Image
    images->outputImageAs(btrgb::TIFF, SP_IMAGE_KEY, this->SP_f_name);
    }catch(std::exception e){
        std::cerr << "Failed to write SP_Image: " << e.what() << std::endl; 
    }
}

void ResultsProcessor::output_btrgb_results(btrgb::ArtObject* images){
    CalibrationResults *calibration_res = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    CalibrationResults *general_info = images->get_results_obj(btrgb::ResultType::GENERAL);
    // Create json object to store all file names this run outputs
    jsoncons::json output_files;
    output_files.insert_or_assign("CM", this->CM_f_name+".tiff");
    output_files.insert_or_assign("SP", this->SP_f_name+".tiff");
    output_files.insert_or_assign("CalibrationResults", this->CalibRes_f_name);
    output_files.insert_or_assign("VerificationResults", this->VerRes_f_name);
    output_files.insert_or_assign("GineralInfo", this->GI_f_name);
    // Add all json objects to the main json body to be writen to .btrgb file
    jsoncons::json btrgb_json;
    btrgb_json.insert_or_assign("OutPutFiles", output_files);
    btrgb_json.insert_or_assign("CalibrationResults", calibration_res->jsonafy());
    btrgb_json.insert_or_assign("VerificationResults", verification_res->jsonafy());
    btrgb_json.insert_or_assign("GeneralInfo", general_info->jsonafy());
    // Write to file
    std::ofstream btrgb_stream;
    btrgb_stream.open(this->output_dir + this->Pro_f_name);
    std::string json_string;
    btrgb_json.dump_pretty(json_string);
    btrgb_stream << json_string;
    btrgb_stream.close();    
}

void ResultsProcessor::output_user_results(btrgb::ArtObject* images){
    CalibrationResults *calibration_res = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    CalibrationResults *general_info = images->get_results_obj(btrgb::ResultType::GENERAL);
    std::string output_dir = images->get_output_dir();
    std::string f_name;

    // TODO the following should be replaced with a way to formate the files as desired by Olivia

    // Write user calibration results
    std::ofstream calibration_stream;
    calibration_stream.open(this->output_dir + this->CalibRes_f_name);
    calibration_res->write_results(calibration_stream);
    calibration_stream.close();
    // Write user verification results
    std::ofstream verification_stream;
    verification_stream.open(this->output_dir + this->VerRes_f_name);
    verification_res->write_results(verification_stream);
    verification_stream.close();
    // Write user General Info
    std::ofstream general_stream;
    general_stream.open(this->output_dir + this->GI_f_name);
    general_info->write_results(general_stream);
    general_stream.close();
}

std::string ResultsProcessor::build_output_name(std::string name, std::string extention){
    std::string f_name = "BTRGB_" + name + "_" + this->ts_id;
    if(name == "")
        f_name = "BTRGB_" + this->ts_id;
    if(extention != "")
        f_name += "." + extention;
    return f_name;
}