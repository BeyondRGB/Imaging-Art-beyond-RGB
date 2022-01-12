//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PreProcessor.h"

PreProcessor::PreProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}

void PreProcessor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    this->callback_func = func;
    this->callback_func("Starting PreProcessor");
    for(const auto& component : this->components){
        component->execute(std::bind(&PreProcessor::my_callback, this, std::placeholders::_1), images);
    }
    this->callback_func("PreProcessor Done!!!");
}

void PreProcessor::my_callback(std::string str) {
    this->callback_func("PreProcessor->" + str);
}


