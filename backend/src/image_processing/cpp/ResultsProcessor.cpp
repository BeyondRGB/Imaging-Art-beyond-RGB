#include "image_processing/header/ResultsProcessor.h"

ResultsProcessor::~ResultsProcessor(){
    if(nullptr != this->formater){
        delete this->formater;
    }
}

void ResultsProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images){
    comms->send_info("","ResultsProcessor");
    comms->send_progress(0,"ResultsProcessor");

    this->output_dir = images->get_output_dir();
    this->ts_id = btrgb::get_time_stamp();

    // Generate the file names to be used for output
    this->CM_f_name = this->build_output_name("CM");
    this->SP_f_name = this->build_output_name("SP");
    // this->CalibRes_f_name = this->build_output_name("Calibration", "csv");
    // this->VerRes_f_name = this->build_output_name("Verification", "csv");
    this->Pro_f_name = this->build_output_name("","btrgb");

    this->GI_f_name = this->build_output_name("GeneralInfo", "txt");
    // this->M_color_f_name = this->build_output_name("M_color", "csv");
    // this->M_spectral_f_name = this->build_output_name("M_spectral", "csv");
    // this->R_ref_f_name = this->build_output_name("R_ref", "csv");
    // this->colorimetry_f_name = this->build_output_name("Colorimetry", "csv");
    // this->R_camera_f_name = this->build_output_name("R_camera", "csv");
    
    // Output Results
    this->output_btrgb_results(images);
    this->output_user_results(images);  

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

    // Write Spectral Image
    try{
        btrgb::Image* sp = images->getImage(SP_IMAGE_KEY);
        CalibrationResults* r = images->get_results_obj(btrgb::ResultType::CALIBRATION);
        sp->setConversionMatrix(BTRGB_M_OPT, r->get_matrix(CM_M));
        sp->setConversionMatrix(BTRGB_OFFSET_OPT, r->get_matrix(CM_OFFSETS));
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
    // output_files.insert_or_assign("CalibrationResults", this->CalibRes_f_name);
    // output_files.insert_or_assign("VerificationResults", this->VerRes_f_name);
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
    cv::Mat xyz_ref = images->get_refrence_data()->xyz_as_matrix();
    CalibrationResults *general_info = images->get_results_obj(btrgb::ResultType::GENERAL);
    CalibrationResults *verification_res = images->get_results_obj(btrgb::ResultType::VERIFICATION);
    CalibrationResults *calibration_res = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    
    
    // std::string output_dir = images->get_output_dir();
    std::string f_name;

    // TODO the following should be replaced with a way to formate the files as desired by Olivia
    
// Write user calibration results
    // M_color
    f_name = this->build_output_name("M_color", "csv");
    this->write_formated_results(f_name, FormatType::M_COLOR, calibration_res);
    // M_spectral
    f_name = this->build_output_name("M_spectral", "csv");
    this->write_formated_results(f_name, FormatType::M_SPECTRAL, calibration_res);
    // R_ref
    f_name = this->build_output_name("R_ref", "csv");

    // Colorimetry
    calibration_res->store_matrix(CM_XYZ_REF, xyz_ref);
    f_name = this->build_output_name("Colorimetry", "csv");
    this->write_formated_results(f_name, FormatType::COLORIMETRY, calibration_res);

    // R_camera

    // Verification
    if(verification_res->contains_results()){
        // Colorimetry
        verification_res->store_matrix(CM_XYZ_REF, xyz_ref);
        f_name = this->build_output_name("ColorimetryVerification", "csv");
        this->write_formated_results(f_name, FormatType::COLORIMETRY, verification_res);
    }


    // Write user verification results
    // std::ofstream verification_stream;
    // verification_stream.open(this->output_dir + this->VerRes_f_name);
    // verification_res->write_results(verification_stream);
    // verification_stream.close();
    // Write user General Info
    std::ofstream general_stream;
    this->set_formater(FormatType::GEN_INFO);
    general_stream.open(this->output_dir + this->GI_f_name);
    this->formater->write_format(general_stream, general_info);
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

void ResultsProcessor::set_formater(FormatType type){
    // Clean up the previous pointer before creating a new one
    if(nullptr != this->formater){
        delete this->formater;
    }

    switch(type){
        case FormatType::GEN_INFO:
            this->formater = new GeneralInfoFormater();
            break;
        case FormatType::M_COLOR:
            this->formater = new MColorFormater();
            break;
        case FormatType::M_SPECTRAL:
            this->formater = new MSpectralFormater();
            break;
        case FormatType::R_REF:
            break;
        case FormatType::COLORIMETRY:
            this->formater = new ColorimetryFormater();
            break;
        case FormatType::R_CAMERA:
            break;
    }
}

void ResultsProcessor::write_formated_results(std::string file_name, FormatType format_type, CalibrationResults *results_obj){
    std::ofstream f_stream;
    f_stream.open(this->output_dir + file_name);
    this->set_formater(format_type);
    this->formater->write_format(f_stream, results_obj);
    f_stream.close();
}