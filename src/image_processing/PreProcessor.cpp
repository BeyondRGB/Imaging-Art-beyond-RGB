//
// Created by ThinkPad41 on 10/10/2021.
//

#include <iostream>
#include "PreProcessor.h"

PreProcessor::PreProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components, int num_components) {
    this->num_components = num_components;
    for(auto & component : components){
        this->components.push_back(component);
    }
}

void PreProcessor::execute() {
    std::cout << "PreProcessor" << std::endl;

    for(const auto& component : this->components){
        component->execute();
    }
}
