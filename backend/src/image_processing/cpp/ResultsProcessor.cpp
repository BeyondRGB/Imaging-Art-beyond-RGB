#include "image_processing/header/ResultsProcessor.h"

void ResultsProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images){
    comms->send_info("","ResultsProcessor");
    comms->send_progress(0,"ResultsProcessor");

    this->output_dir = images->get_output_dir();
    
    this->output_user_results(images);  
    this->output_btrgb_results(images);

    comms->send_progress(0.2,"ResultsProcessor");  
    this->output_images(images);
    
    comms->send_progress(1,"ResultsProcessor");
}

void ResultsProcessor::output_images(btrgb::ArtObject* images){
    for(const auto& [name, img]: *images) {
        images->outputImageAs(btrgb::TIFF, name, name);
    }
}

void ResultsProcessor::output_btrgb_results(btrgb::ArtObject* images){
    CalibrationResults *calibration_res = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    CalibrationResults *general_info = images->get_results_obj(btrgb::ResultType::GENERAL);

    jsoncons::json btrgb_json;
    btrgb_json.insert_or_assign("CalibrationResults", calibration_res->jsonafy());
    btrgb_json.insert_or_assign("VerificationResults", verification_res->jsonafy());
    btrgb_json.insert_or_assign("GeneralInfo", general_info->jsonafy());

    std::ofstream btrgb_stream;
    btrgb_stream.open(this->output_dir + "Results.btrgb");
    std::string json_string;
    btrgb_json.dump_pretty(json_string);
    std::cout << "Writing Results.btrgb" << std::endl << json_string << std::endl;
    btrgb_stream << json_string;
    btrgb_stream.close();    
}

void ResultsProcessor::output_user_results(btrgb::ArtObject* images){
    CalibrationResults *calibration_res = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    CalibrationResults *general_info = images->get_results_obj(btrgb::ResultType::GENERAL);
    std::string output_dir = images->get_output_dir();

    std::ofstream calibration_stream;
    calibration_stream.open(this->output_dir + "CalibrationResults.csv");
    calibration_res->write_results(calibration_stream);
    calibration_stream.close();

    std::ofstream verification_stream;
    verification_stream.open(this->output_dir + "VerificationResults.csv");
    verification_res->write_results(verification_stream);
    verification_stream.close();

    std::ofstream general_stream;
    general_stream.open(this->output_dir + "GeneralInfo.txt");
    general_info->write_results(general_stream);
    general_stream.close();
}