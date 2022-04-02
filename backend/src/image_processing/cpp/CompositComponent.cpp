// #include "image_processing/header/CompositComponent.h"

// std::string CompositComponent::get_component_list(){
//     std::string component_list = "{\"" + this->get_name() + "\":[";
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

// void CompositComponent::init_components(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components){
//     for(auto & component : components){
//         this->components.push_back(component);
//     }
// }