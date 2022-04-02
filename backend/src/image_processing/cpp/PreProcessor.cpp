//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PreProcessor.h"

PreProcessor::PreProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components)
    : CompositComponent("PreProcessor") {
        this->init_components(components);
}

void PreProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting PreProcessing", "PreProcessor");
    double count = 0;
    double total = this->components.size();
    for(const auto& component : this->components){
      double currProgress = count / total;
      comms->send_progress(currProgress, "PreProcessor");
      component->execute(comms, images);
      comms->send_base64(images->getImage("art1"), btrgb::FAST);
      count++;
    }
    comms->send_info("PreProcessing Done!!!", "PreProcessor");
}

// std::string PreProcessor::get_component_list(){
//     std::string component_list = "{\"PreProcessor\":[";
//     int i = 0;
//     for(auto  & component : this->components){
//         if(i > 0){
//             component_list += ",";
//         }
//         component_list += component->get_component_list();
//         i++;
//     }
//     component_list += "]}";
//     return component_list;
// }
