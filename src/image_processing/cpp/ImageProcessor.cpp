//
// Created by ThinkPad41 on 10/10/2021.
//


#include "../header/ImageProcessor.h"

ImageProcessor::ImageProcessor(const vector<shared_ptr<ImgProcessingComponent>> &components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}
void ImageProcessor::execute(callback callback_func) {
    callback_func("Image Processor");
    for(auto  & component : this->components){
        component->execute(callback_func);
    }
}