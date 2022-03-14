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
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new RawImageReader()));
    //pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ChannelSelector()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new BitDepthScaler()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new FlatFieldor()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new PixelRegestor()));
    //Set up Calibration components
    std::vector<std::shared_ptr<ImgProcessingComponent>> calibration_components;
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ColorManagedCalibrator()));
    //calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new SpectralCalibrator()));

    std::vector<std::shared_ptr<ImgProcessingComponent>> img_process_components;
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new PreProcessor(pre_process_components)));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ImageCalibrator(calibration_components)));

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
        art_obj->setTargetInfo(td);
        return true;
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return false;
}

void Pipeline::run() {

    this->send_info("I got your msg", this->get_process_name());
    this->send_info( this->process_data_m->to_string(), this->get_process_name());
    std::shared_ptr<ImgProcessingComponent> pipeline = pipelineSetup();

    std::string ref_file = this->get_ref_file();
    IlluminantType illuminant = this->get_illuminant_type();
    ObserverType observer = this->get_observer_type();

    btrgb::ArtObject* images;
    try {
        images = new  btrgb::ArtObject(ref_file, illuminant, observer);
    }
    catch(const std::exception& err) {
        std::string msg(err.what());
        this->report_error(this->get_process_name(), "[art_obj construction]: " + msg);
    }
    catch(...) {
        this->report_error(this->get_process_name(), "Some other error occured during ArtObject construction.");
    }


    this->send_info("About to init art obj...", this->get_process_name());
    this->init_art_obj(images);



    this->send_info( "About to execute...", this->get_process_name());
    try {
        pipeline->execute(this->coms_obj_m.get(), images);
    }
    catch(const std::exception& err) {
        std::string msg(err.what());
        this->report_error(this->get_process_name(), "[pipeline execution]: " + msg);
    }
    catch(...) {
        this->report_error(this->get_process_name(), "Some other error occured during pipeline execution.");
    }


    for(const auto& [name, img]: *images) {
        images->outputImageAs(btrgb::TIFF, name, name);
    }

    delete images;

}

IlluminantType Pipeline::get_illuminant_type() {
    // Default to D50
    IlluminantType type = IlluminantType::D50;
    try {
        Json ref_data = this->process_data_m->get_obj(key_map[DataKey::RefData]);
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
        Json ref_data = this->process_data_m->get_obj(key_map[DataKey::RefData]);
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
        Json ref_data = this->process_data_m->get_obj(key_map[DataKey::RefData]);
        ref_file = ref_data.get_string("name");
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
        throw e;
    }
    return ref_file;
}
