#include "ImageUtil/ArtObject.hpp"
#include "pipeline.hpp"


std::shared_ptr<ImgProcessingComponent> Pipeline::pipelineSetup() {
    //Set up PreProcess components
    std::vector<std::shared_ptr<ImgProcessingComponent>> pre_process_components;
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ImageReader()));
    //pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ChannelSelector()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new BitDepthScaler()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new FlatFieldor()));
    //Sharpening and Noise Reduction
    if(this->get_sharpen_type() != "N"){
        pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new NoiseReduction(this->get_sharpen_type())));
    }
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new PixelRegestor(this->get_registration_type())));
    //Set up Calibration components
    std::vector<std::shared_ptr<ImgProcessingComponent>> calibration_components;
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ColorManagedCalibrator()));
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new SpectralCalibrator()));
   if(this->should_verify){
        calibration_components.push_back(std::shared_ptr<ImgProcessingComponent>(new Verification())) ;     
    }
    calibration_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ResultsProcessor()));
 
    std::vector<std::shared_ptr<ImgProcessingComponent>> img_process_components;
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new PreProcessor(pre_process_components)));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ImageCalibrator(calibration_components)));
    
    return std::shared_ptr<ImgProcessingComponent>(new ImageProcessor(img_process_components));

};

bool Pipeline::init_art_obj(btrgb::ArtObject* art_obj) {
    try {
        // Extract Image Array from request data
        Json image_array = this->process_data_m->get_array(key_map[DataKey::IMAGES]);
        
        std::cout << image_array.to_string(true) << std::endl;
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
            try{
                std::string target_file = obj.get_string(key_map[DataKey::TARGET_IMG]);
                art_obj->newImage(("target" + std::to_string(i + 1)), target_file);
            }catch(ParsingError e){ /* No target provided. We expect the target to be in the art image */ }
        }
        //Collect the information provided about the color target
        // TargetData td;
        Json target_location = this->process_data_m->get_obj(key_map[DataKey::TargetLocation]);
        TargetData td = this->build_target_data(target_location);
        art_obj->setTargetInfo(td);
        this->send_info("TargetData initialized:", this->get_process_name());

        // set custom output file name
        art_obj->setOutputFileName(this->process_data_m->get_string(key_map[DataKey::OUT_FILE_NAME]));
        
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
    Json target_json = this->process_data_m->get_obj(key_map[DataKey::TargetLocation]);
    Json ref_data_json = target_json.get_obj(key_map[DataKey::ReferenceData]);
    results_obj->store_string(GI_TARGET_ID, ref_data_json.get_string("fileName"));
    // Target Dims
    results_obj->store_int(GI_TARGET_ROWS, target_json.get_number("rows"));
    results_obj->store_int(GI_TARGET_COLS, target_json.get_number("cols"));
    // Target Coords
    results_obj->store_double(GI_TARGET_TOP, target_json.get_number("top")*target_json.get_number("resolution"));
    results_obj->store_double(GI_TARGET_BOTTOM, target_json.get_number("bottom") * target_json.get_number("resolution"));
    results_obj->store_double(GI_TARGET_LEFT, target_json.get_number("left") * target_json.get_number("resolution"));
    results_obj->store_double(GI_TARGET_RIGHT, target_json.get_number("right") * target_json.get_number("resolution"));
    // Observer
    int observer_num = ref_data_json.get_number(key_map[DataKey::StandardObserver]);
    results_obj->store_int(GI_OBSERVER, observer_num);
    // Illuminant
    std::string illum_str = ref_data_json.get_string(key_map[DataKey::Illuminants]);
    results_obj->store_string(GI_ILLUMINANT, illum_str);
    // White Patch Coords
    RefData *ref_data = art_obj->get_refrence_data();
    TargetData td = build_target_data(target_json);
    std::string coords = ref_data->get_color_patch(td.w_row, td.w_col)->get_name();
    results_obj->store_string(GI_WHITE_PATCH_COORDS, coords);
    // Store input images
    for(const auto& [key, im] : *art_obj){
        results_obj->store_string(key, im->getName());
    }
    // Store Filtering Options
    std::string option = this->get_sharpen_type();
    std::string option_string = "None";
    if(option == "H")
        option_string = "High";
    else if(option == "M")
        option_string = "Medium";
    else if(option == "L")
        option_string = "Low";
    results_obj->store_string(GI_ADVANCED_FILTERS, option_string);

}

void Pipeline::run() {
    std::cout << "Initializing Img Processing Pipeline" << std::endl;
    this->send_info("I got your msg", this->get_process_name());
    this->send_info( this->process_data_m->to_string(), this->get_process_name());

    std::string out_dir;

    std::string art_image;

    bool batch = this->process_data_m->get_bool("batch");

    //Gets the name of the Art image to name output directories
    Json image_array = this->process_data_m->get_array(key_map[DataKey::IMAGES]);
    Json obj = image_array.obj_at(0);
    std::string art_file = obj.get_string(key_map[DataKey::ART]);
    std::filesystem::path p(art_file);
    std::string filenameWithoutExtension = p.stem().string();
    size_t lastUnderscorePosition = filenameWithoutExtension.rfind('_');
    if (lastUnderscorePosition != std::string::npos) {
        filenameWithoutExtension = filenameWithoutExtension.substr(0, lastUnderscorePosition);
    }


    if (batch) {
        try {
            out_dir = this->process_data_m->get_string("outputDirectory");
            std::cout << out_dir << std::endl;

            std::filesystem::path fsPath(out_dir);
            // Navigate two levels up
            std::filesystem::path parentPath = fsPath.parent_path().parent_path();
            // Use filesystem path to append filename, ensuring correct path separators
            std::filesystem::path finalPath = parentPath / filenameWithoutExtension;
            std::string original_dir = finalPath.string();

            std::filesystem::path path{ original_dir };
            int counter = 1;
            while (std::filesystem::exists(path)) {
                // Append a number to make the directory unique, using filesystem to handle path
                path = parentPath / (filenameWithoutExtension + "_" + std::to_string(counter++));
            }
            std::filesystem::create_directories(path);
            out_dir = path.string();
            std::cout << out_dir << std::endl;
        }
   
        catch (const ParsingError&) {

        }
    }
    else {
        try {
            out_dir = this->get_output_directory(filenameWithoutExtension);
        }
        catch (...) {
            return;
        }
    }


    /* Create ArtObject */
    std::unique_ptr<btrgb::ArtObject> images;
    try { 
        Json target_data = this->process_data_m->get_obj(key_map[DataKey::TargetLocation]);
        std::string ref_file = this->get_ref_file(target_data);
        IlluminantType illuminant = this->get_illuminant_type(target_data);
        ObserverType observer = this->get_observer_type(target_data);
        images.reset(new  btrgb::ArtObject(ref_file, illuminant, observer, out_dir,batch)); 
    }catch(RefData_FailedToRead e){
        this->report_error(this->get_process_name(), e.what());
        return;
    }catch(RefData_ParssingError e){
        this->report_error(this->get_process_name(), e.what());
        return;
    }catch(const std::exception& err) {
        this->report_error(this->get_process_name(), err.what());
        return;
    }


    /* Initialize ArtObject with request data */
    this->send_info("About to init art obj...", this->get_process_name());
    try{
        this->init_art_obj(images.get());
        this->init_verification(images.get());
    }catch(std::exception e){
        this->report_error(this->get_process_name(), e.what());
        return;
    }

    // Initialize General Info Results
    this->send_info("About to init GeneralInfo...", this->get_process_name());
    try{
        this->init_general_info(images.get());
    }catch(std::exception e){
        this->report_error(this->get_process_name(), e.what());
        return;
    }

    // Verify Targets
    this->send_info("Verifying ColorTargets...", this->get_process_name());
    if( !this->verify_targets(images.get()) ){
        return;
    }

    /* Execute the pipeline on the created ArtObject */
    std::shared_ptr<ImgProcessingComponent> pipeline = pipelineSetup();
    this->send_info( "About to execute...", this->get_process_name());
    try { 
        this->coms_obj_m->send_pipeline_components(pipeline->get_component_list());
        pipeline->execute(this->coms_obj_m.get(), images.get());
        std::string Pro_file = images.get()->get_results_obj(btrgb::ResultType::GENERAL)->get_string(PRO_FILE);
        this->coms_obj_m->send_post_calibration_msg(Pro_file);
    } catch(const ImgProcessingComponent::error& e) {
        this->report_error(e.who(), e.what());
        return;
    }
    catch(ColorTarget_MissmatchingRefData e){
        this->report_error(this->get_process_name(), e.what());
        return;
    }catch(const std::exception& err) {
        this->report_error(this->get_process_name(), err.what());
        return;
    }

}


std::string Pipeline::get_output_directory(std::string artImage) {

    std::time_t now = std::time(0);
    std::tm* ltm = std::localtime(&now);
    std::string date_string = btrgb::get_date("-");
    std::string time_string = btrgb::get_time(btrgb::TimeType::MILITARY, "-");
    try {
        std::string base_dir = this->process_data_m->get_string("destinationDirectory");
        std::string dir = base_dir + "/" + OUTPUT_PREFIX + date_string + "_" + time_string + "/";
        dir = dir + "/" + artImage;
        std::filesystem::create_directories(dir);
        return dir;
    }
    catch (ParsingError e) {
        this->report_error("[Pipeline]", "Process request: invalid or missing \"destinationDirectory\" field.");
        throw;
    }
    catch (const std::filesystem::filesystem_error& err) {
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



std::string Pipeline::get_registration_type() {

    //default to no sharpening
    std::string registration_string = "M";
    try {
        registration_string = this->process_data_m->get_string("sharpenString");
        if (registration_string == "H" || registration_string == "M" || registration_string == "L") {
            return registration_string;
        }
    }
    catch (ParsingError e) {
    }
    return registration_string;
}



IlluminantType Pipeline::get_illuminant_type(Json target_data) {
    // Defaults to D50
    IlluminantType type = RefData::get_illuminant("");
    try {
        Json ref_data = target_data.get_obj(key_map[DataKey::ReferenceData]);
        std::string illum_str = ref_data.get_string(key_map[DataKey::Illuminants]);
        type = RefData::get_illuminant(illum_str);
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return type;
}

ObserverType Pipeline::get_observer_type(Json target_data) {
    // Defailts to 1931
    ObserverType type = RefData::get_observer(1931);
    try {
        Json ref_data = target_data.get_obj(key_map[DataKey::ReferenceData]);
        int observer_num = ref_data.get_number(key_map[DataKey::StandardObserver]);
        type = RefData::get_observer(observer_num);
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return type;

}

TargetData Pipeline::build_target_data(Json target_json){
    TargetData td;
    std::cout << target_json.to_string(true) << std::endl;
    td.top_loc = target_json.get_number("top");
    td.left_loc = target_json.get_number("left");
    td.bot_loc = target_json.get_number("bottom");
    td.right_loc = target_json.get_number("right");
    td.col_count = target_json.get_number("cols");
    td.row_count = target_json.get_number("rows");
    td.sample_size = target_json.get_number("size");
    Json white_loc = target_json.get_obj("whitePatch");
    // subtract one to make it zero based, due to front end sending 1 based.
    td.w_row = white_loc.get_number("row") - 1;
    td.w_col = white_loc.get_number("col") - 1;
    Json ref_loc = target_json.get_obj("refData");
    td.ref_base = ref_loc.get_string("fileName");
    td.illum_base = ref_loc.get_string("illuminants");
    td.obsv_base = ref_loc.get_number("standardObserver");
    return td;
}

std::string Pipeline::get_ref_file(Json target_data) {
    std::string ref_file = "";
    try {
        Json ref_data = target_data.get_obj(key_map[DataKey::ReferenceData]);
        ref_file = ref_data.get_string("fileName");
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
        throw e;
    }
    return ref_file;
}

void Pipeline::init_verification(btrgb::ArtObject* images){
    try{
        Json verification_json = this->process_data_m->get_obj(key_map[DataKey::VerificationLocation]);
        TargetData vd = this->build_target_data(verification_json);
        images->init_verification_data(vd);
        this->should_verify = true;
        this->send_info("Verivication Targe Was Provided", this->get_process_name());

    }catch(ParsingError e){
        this->send_info("No Verification Target provided", this->get_process_name());
        this->should_verify = false;
    }
}

bool Pipeline::verify_targets(btrgb::ArtObject *images){
    try{
        // Test Target
        images->get_target(TARGET(1), btrgb::TargetType::GENERAL_TARGET);
        images->get_target(TARGET(1), btrgb::TargetType::VERIFICATION_TARGET);
    }catch(ColorTarget_MissmatchingRefData e){
        this->report_error(this->get_process_name(), e.what());
        return false;
    }catch(btrgb::ArtObj_VerificationDataNull){}
    return true;
}
