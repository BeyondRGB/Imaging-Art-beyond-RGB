//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PreProcessor.h"

PreProcessor::PreProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components) {
    for(auto & component : components){
        this->components.push_back(component);
    }
}

void PreProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting PreProcessing", "PreProcessor");
    double count = 0;
    double total = this->components.size();
    for(const auto& component : this->components){
      double currProgress = count / total;
      comms->send_progress(currProgress, "PreProcessor");
      component->execute(comms, images);
      count++;
    }
    comms->send_info("PreProcessing Done!!!", "PreProcessor");
}
