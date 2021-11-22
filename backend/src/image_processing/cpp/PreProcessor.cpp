//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PreProcessor.h"

PreProcessor::PreProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}

void PreProcessor::execute(CallBackFunction func, ArtObject* images) {
    this->callback_func = func;
    this->callback_func("Starting PreProcessor");
    for(const auto& component : this->components){
        component->execute(std::bind(&PreProcessor::my_callback, this, placeholders::_1), images);
    }
    this->callback_func("PreProcessor Done!!!");
}

void PreProcessor::my_callback(string str) {
    this->callback_func("PreProcessor->" + str);
}


