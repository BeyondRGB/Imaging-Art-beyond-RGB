//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ImageCalibrator.h"

ImageCalibrator::ImageCalibrator(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}
void ImageCalibrator::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting Image Calibration", "ImageCalibrator");
    double count = 0;
    double total = this->components.size();
    for(auto  & component : this->components){
        double currProgress = count / total;
        comms->send_progress(currProgress, "ImageCalibrator");
        component->execute(comms, images);
        comms->send_base64(images->getImage("ColorManaged"), btrgb::FAST);
        count++;
    }
    comms->send_info("Image Calibration Done!!!", "ImageCalibrator");
}

std::string ImageCalibrator::get_component_list(){
    std::string component_list = "{\"ImageCalibrator\":[";
    int i = 0;
    for(auto  & component : this->components){
        if(i > 0){
            component_list += ",";
        }
        component_list += component->get_component_list();
        i++;
    }
    component_list += "]}";
    return component_list;
}