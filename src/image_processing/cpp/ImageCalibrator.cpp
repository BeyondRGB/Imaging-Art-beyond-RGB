//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ImageCalibrator.h"

ImageCalibrator::ImageCalibrator(const vector<shared_ptr<ImgProcessingComponent>> &components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}
void ImageCalibrator::execute(CallBackFunction func) {
    func("Starting Image Calibration");
    for(auto  & component : this->components){
        component->execute(func);
    }
    func("Image Calibration Done!!!");
}