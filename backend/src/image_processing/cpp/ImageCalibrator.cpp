//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ImageCalibrator.h"

ImageCalibrator::ImageCalibrator(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}
void ImageCalibrator::execute(CallBackFunction func, btrgb::ArtObject* images) {
    this->callback_func = func;
    this->callback_func("Starting Image Calibration");
    for(auto  & component : this->components){
        component->execute(std::bind(&ImageCalibrator::my_callback, this, std::placeholders::_1), images);
    }
    this->callback_func("Image Calibration Done!!!");
}

void ImageCalibrator::my_callback(std::string str) {
    this->callback_func("ImageCalibrator->" + str);
}