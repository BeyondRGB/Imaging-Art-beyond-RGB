//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ImageProcessor.h"

ImageProcessor::ImageProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}
void ImageProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting Image Processor", "ImageProcessor");
    double count = 0;
    double total = this->components.size();
    for(auto  & component : this->components){
        double currProgress = count / total;
        comms->send_progress(currProgress, "ImageProcessor");
        component->execute(comms, images);
        comms->send_base64(images->getImage("art1"), btrgb::PNG, btrgb::FAST);
        count++;
    }
    comms->send_info("Image Processing Done!!!", "ImageProcessor");
}
