//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PreProcessor.h"

PreProcessor::PreProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}

void PreProcessor::execute(callback callback_func) {
    callback_func("PreProcessor");
    for(const auto& component : this->components){
        component->execute(callback_func);
    }
}
