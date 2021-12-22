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
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new BitDepthScalor()));
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
    /* Demo
    images->newImage("test", "nikon_targets_2.NEF");
    */

    pipeline->execute(std::bind(&Pipeline::callback, this, std::placeholders::_1), images);

    /* Demo
    images->outputImageAsTIFF("test");
    */
    
    delete images;

};