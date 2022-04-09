#include "ImageUtil/ArtObject.hpp"
#include "pipeline.hpp"

int Pipeline::pipeline_count = 0;

Pipeline::Pipeline(){
    pipeline_count++;
    num_m = pipeline_count;
    this->set_process_name("Img Processing Pipeline (" + std::to_string(num_m) + ")");
};

void Pipeline::callback(std::string msg) {
    msg = "{pipeline(" + std::to_string(num_m) + "):" + msg + "}";
    std::cout << "MSG: " << msg << std::endl;
    this->send_info(msg, this->get_process_name());
};

std::shared_ptr<ImgProcessingComponent> Pipeline::pipelineSetup() {
    //Set up PreProcess components
    std::vector<std::shared_ptr<ImgProcessingComponent>> pre_process_components;
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ImageReader()));
    //pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ChannelSelector()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new BitDepthScaler()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new FlatFieldor()));
    //Sharpening and Noise Reduction
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new NoiseReduction(get_sharpen_type())));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new PixelRegestor()));
    //Set up Calibration components
    std::vector<std::shared_ptr<ImgProcessingComponent>> calibration_components;
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ColorManagedCalibrator()));
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new SpectralCalibrator()));

    std::vector<std::shared_ptr<ImgProcessingComponent>> img_process_components;
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new PreProcessor(pre_process_components)));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ImageCalibrator(calibration_components)));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ResultsProcessor()));

    return std::shared_ptr<ImgProcessingComponent>(new ImageProcessor(img_process_components));

};

bool Pipeline::init_art_obj(btrgb::ArtObject* art_obj) {
    try {
        // Extract Image Array from request data
        Json image_array = this->process_data_m->get_array(key_map[DataKey::IMAGES]);
        for (int i = 0; i < image_array.get_size(); i++) {
            // Extract each obj from array
            Json obj = image_array.obj_at(i);
            // Extract each image file name from current object
            std::string art_file = obj.get_string(key_map[DataKey::ART]);
            std::string white_file = obj.get_string(key_map[DataKey::WHITE]);
            std::string dark_file = obj.get_string(key_map[DataKey::DARK]);
            // Add each file to the ArtObject
            art_obj->newImage(("art" + std::to_string(i + 1)), art_file);
            art_obj->newImage(("white" + std::to_string(i + 1)), white_file);
            art_obj->newImage(("dark" + std::to_string(i + 1)), dark_file);
        }
        //Collect the information provided about the color target
        TargetData td;
        Json target_location = this->process_data_m->get_obj(key_map[DataKey::TargetLocation]);
        td.top_loc = target_location.get_number("top");
        td.left_loc = target_location.get_number("left");
        td.bot_loc = target_location.get_number("bottom");
        td.right_loc = target_location.get_number("right");
        td.col_count = target_location.get_number("cols");
        td.row_count = target_location.get_number("rows");
        td.sample_size = target_location.get_number("size");
        art_obj->setTargetInfo(td);
        return true;
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return false;
}

void Pipeline::init_general_info(btrgb::ArtObject* art_obj){
    CalibrationResults *results_obj = art_obj->get_results_obj(btrgb::ResultType::GENERAL);
    // Make/Model
    // TODO add this once info sent from front end
    // Target ID
    Json ref_data_json = this->process_data_m->get_obj(key_map[DataKey::ReferenceData]);
    results_obj->store_string(GI_TARGET_ID, ref_data_json.get_string("name"));
    // Target Dims
    Json target_json = this->process_data_m->get_obj(key_map[DataKey::TargetLocation]);
    results_obj->store_int(GI_TARGET_ROWS, target_json.get_number("rows"));
    results_obj->store_int(GI_TARGET_COLS, target_json.get_number("cols"));
    // Observer
    int observer_num = ref_data_json.get_number(key_map[DataKey::StandardObserver]);
    results_obj->store_int(GI_OBSERVER, observer_num);
    // Illuminant
    std::string illum_str = ref_data_json.get_string(key_map[DataKey::Illuminants]);
    results_obj->store_string(GI_ILLUMINANT, illum_str);
    // White Patch Coords
    RefData *ref_data = art_obj->get_refrence_data();
    std::string coords = ref_data->get_white_patch()->get_name();
    results_obj->store_string(GI_WHITE_PATCH_COORDS, coords);

}

void Pipeline::run() {

    this->send_info("I got your msg", this->get_process_name());
    this->send_info( this->process_data_m->to_string(), this->get_process_name());
    std::shared_ptr<ImgProcessingComponent> pipeline = pipelineSetup();

    std::string ref_file = this->get_ref_file();
    IlluminantType illuminant = this->get_illuminant_type();
    ObserverType observer = this->get_observer_type();
    std::string out_dir;
    try{out_dir = this->get_output_directory();}
    catch(...) {return;}


    /* Create ArtObject */
    std::unique_ptr<btrgb::ArtObject> images;
    try { images.reset(new  btrgb::ArtObject(ref_file, illuminant, observer, out_dir)); }
    catch(const std::exception& err) {
        this->report_error(this->get_process_name(), err.what());
        return;
    }


    /* Initialize ArtObject with request data */
    this->send_info("About to init art obj...", this->get_process_name());
    this->init_art_obj(images.get());

    // Initialize General Info Results
    this->init_general_info(images.get());

    /* Execute the pipeline on the created ArtObject */
    this->send_info( "About to execute...", this->get_process_name());
    try { 
        pipeline->execute(this->coms_obj_m.get(), images.get());
    }catch(const std::exception& err) {
        this->report_error(this->get_process_name(), err.what());
        return;
    }

}


std::string Pipeline::get_output_directory() {
    
	std::time_t now = std::time(0);
	std::tm *ltm = std::localtime(&now);
    std::string date_string = btrgb::get_date("-");
    std::string time_string = btrgb::get_time(btrgb::TimeType::MILITARY, "-");
    try {
        std::string base_dir = this->process_data_m->get_string("destinationDirectory");
        std::string dir = base_dir + "/BTRGB_" + date_string + "_" + time_string + "/";
        std::filesystem::create_directories(dir);
        return dir;
    }
    catch (ParsingError e) {
        this->report_error("[Pipeline]", "Process request: invalid or missing \"destinationDirectory\" field.");
        throw;
    }
    catch(const std::filesystem::filesystem_error& err) {
        this->report_error("[Pipeline]", "Failed to create or access output directory.");
        throw;
    }


}

std::string Pipeline::get_sharpen_type() {

    //default to no sharpening
    std::string sharpen_string = "N";
    try {
        sharpen_string = this->process_data_m->get_string("sharpenString");
        if (sharpen_string == "H" || sharpen_string == "M" || sharpen_string == "L" || sharpen_string == "N") {
            return sharpen_string;
        }
    }
    catch (ParsingError e) {  
    }
    return sharpen_string;
}



IlluminantType Pipeline::get_illuminant_type() {
    // Default to D50
    IlluminantType type = IlluminantType::D50;
    try {
        Json ref_data = this->process_data_m->get_obj(key_map[DataKey::ReferenceData]);
        std::string illum_str = ref_data.get_string(key_map[DataKey::Illuminants]);
        if (illum_str == "A") {
            type = IlluminantType::A;
        }
        if (illum_str == "D65") {
            type = IlluminantType::D65;
        }
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return type;
}

ObserverType Pipeline::get_observer_type() {
    // Defailt to 1931
    ObserverType type = ObserverType::SO_1931;
    try {
        Json ref_data = this->process_data_m->get_obj(key_map[DataKey::ReferenceData]);
        int observer_num = ref_data.get_number(key_map[DataKey::StandardObserver]);
        if (observer_num == 1964) {
            type = ObserverType::SO_1964;
        }
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return type;

}

std::string Pipeline::get_ref_file() {
    std::string ref_file = "";
    try {
        Json ref_data = this->process_data_m->get_obj(key_map[DataKey::ReferenceData]);
        ref_file = ref_data.get_string("name");
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
        throw e;
    }
    return ref_file;
}
