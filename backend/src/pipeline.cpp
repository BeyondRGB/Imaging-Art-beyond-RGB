#include "ImageUtil/ArtObject.hpp"
#include "pipeline.h"

Pipeline::Pipeline(server* s, websocketpp::connection_hdl hdl, message_ptr msg, int pipelineNumber) {
    server_m = s;
    connectionHandle_m = hdl;
    msg_m = msg;
    num_m = pipelineNumber;
};

void Pipeline::callback(std::string msg) {
    msg = "{pipeline(" + std::to_string(num_m) + "):" + msg + "}";
    std::cout << "MSG: " << msg << std::endl;
    server_m->send(connectionHandle_m, msg, msg_m->get_opcode());

};

std::shared_ptr<ImgProcessingComponent> Pipeline::pipelineSetup() {
    //Set up PreProcess components
    std::vector<std::shared_ptr<ImgProcessingComponent>> pre_process_components;
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new RawImageReader()));
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

void Pipeline::executePipeline() {
    callback("I got your msg");
    std::shared_ptr<ImgProcessingComponent> pipeline = pipelineSetup();
    
    
    btrgb::ArtObject* images = new  btrgb::ArtObject();

    /* =====[ DEMO ]=========
    #include <map>
    std::map<std::string, std::string> files = {
        {"dark1", "nikon_dark_1.NEF"},
        {"dark2", "nikon_dark_2.NEF"},
        {"white1", "nikon_white_1.NEF"},
        {"white2", "nikon_white_2.NEF"},
        {"art1", "nikon_targets_1.NEF"},
        {"art2", "nikon_targets_2.NEF"}
    };
    for(const auto& [type, file]: files)
        images->newImage(type, file);
    */

    pipeline->execute(std::bind(&Pipeline::callback, this, std::placeholders::_1), images);
    
    /* =====[ DEMO ]=========
    for(const auto& [type, file]: files)
        images->outputImageAsTIFF(type);
    */
    
    delete images;

};