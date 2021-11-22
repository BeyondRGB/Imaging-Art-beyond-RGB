//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ImageCalibrator.h"

ImageCalibrator::ImageCalibrator(const vector<shared_ptr<ImgProcessingComponent>> &components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}
void ImageCalibrator::execute(CallBackFunction func, ArtObject* images) {
    this->callback_func = func;
    this->callback_func("Starting Image Calibration");
    for(auto  & component : this->components){
        component->execute(std::bind(&ImageCalibrator::my_callback, this, placeholders::_1), images);
    }
    this->callback_func("Image Calibration Done!!!");
}

void ImageCalibrator::my_callback(string str) {
    this->callback_func("ImageCalibrator->" + str);
}