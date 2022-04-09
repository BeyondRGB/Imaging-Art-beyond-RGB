//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ImageCalibrator.h"

ImageCalibrator::ImageCalibrator(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components)
     : CompositComponent("ImageCalibrator"){
        this->init_components(components);
}
void ImageCalibrator::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting Image Calibration", "ImageCalibrator");
    double count = 0;
    double total = this->components.size();
    for(auto  & component : this->components){
        double currProgress = count / total;
        comms->send_progress(currProgress, "ImageCalibrator");
        component->execute(comms, images);
        comms->send_binary(images->getImage("ColorManaged"), btrgb::FAST);
        count++;
    }
    comms->send_info("Image Calibration Done!!!", "ImageCalibrator");
}
