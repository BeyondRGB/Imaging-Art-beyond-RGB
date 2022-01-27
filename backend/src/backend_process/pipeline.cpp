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
    this->send_msg(msg);
};

std::shared_ptr<ImgProcessingComponent> Pipeline::pipelineSetup() {
    //Set up PreProcess components
    std::vector<std::shared_ptr<ImgProcessingComponent>> pre_process_components;
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new RawImageReader("LibRaw")));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ChannelSelector()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new BitDepthScaler()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new DarkCurrentCorrector()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new FlatFeildor()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new PixelRegestor()));
    //Set up Calibration components
    std::vector<std::shared_ptr<ImgProcessingComponent>> calibration_components;
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ColorManagedCalibrator()));
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new SpectralCalibrator()));

    auto* process = new PreProcessor(pre_process_components);
    auto* calibrator = new ImageCalibrator(calibration_components);

    std::vector<std::shared_ptr<ImgProcessingComponent>> img_process_components;
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(process));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(calibrator));

    ImgProcessingComponent* processor = new ImageProcessor(img_process_components);
    return std::shared_ptr<ImgProcessingComponent>(processor);

};

bool Pipeline::init_art_obj(btrgb::ArtObject* art_obj) {
    try {
        // Extract Image Array from request data
        Json image_array = this->process_data_m->get_array(key_map[ImageKey::IMAGES]);
        for (int i = 0; i < image_array.get_size(); i++) {
            // Extract each obj from array
            Json obj = image_array.obj_at(i);
            // Extract each image file name from current object
            std::string art_file = obj.get_string(key_map[ImageKey::ART]);
            std::string white_file = obj.get_string(key_map[ImageKey::WHITE]);
            std::string dark_file = obj.get_string(key_map[ImageKey::BLACK]);
            // Add each file to the ArtObject
            art_obj->newImage(("art" + std::to_string(i + 1)), art_file);
            art_obj->newImage(("white" + std::to_string(i + 1)), white_file);
            art_obj->newImage(("black" + std::to_string(i + 1)), dark_file);
        }
        //Stuff here is pending the implementation of how the target is being defined
        Json target_location = this->process_data_m->process_data_m->get_obj("TargetLocation");
        Json tLeft = target_location.get_obj("TopLeft")
        double tLeftX = tLeft.get_number("x");
        double tLeftY = tLeft.get_number("y");
        Json tRight = target_location.get_obj("TopRight");
        double tRightX = tRight.get_number("x");
        double tRightY = tRight.get_number("y");
        Json bRight = target_location.get_obj("BottomRight");
        double bRightX = bRight.get_number("x");
        double bRightY = bRight.get_number("y");
        Json bLeft = target_location.get_obj("BottomLeft");
        double bLeftX = bLeft.get_number("x");
        double bLeftY = bLeft.get_number("y");
        int numRows = target_location("NumRows");
        int numCols = target_location("NumCols");
        art_obj->targetInfo(tLeftX, tLeftY, tRightX, tRightY, bRightX, bRightY, bLeftX, bLeftY, numRows, numCols);
        return true;
    }
    catch (ParsingError e) {
        std::string name = this->get_process_name();
        this->report_error(name, e.what());
    }
    return false;
}

void Pipeline::run() {
    this->send_msg("I got your msg");
    this->send_msg(this->process_data_m->to_string());
    std::shared_ptr<ImgProcessingComponent> pipeline = pipelineSetup();



    btrgb::ArtObject* images = new  btrgb::ArtObject();
    this->init_art_obj(images);


    pipeline->execute(std::bind(&Pipeline::callback, this, std::placeholders::_1), images);


    for(const auto& [name, img]: *images) {
        images->outputImageAsTIFF(name);
    }

    delete images;

}
