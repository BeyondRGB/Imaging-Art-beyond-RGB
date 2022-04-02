#ifndef COMPOSIT_COMPONENT_H
#define COMPOSIT_COMPONENT_H

#include "image_processing/header/ImgProcessingComponent.h"

class CompositComponent : public ImgProcessingComponent{
    public:
        CompositComponent(std::string name) : ImgProcessingComponent(name){}
        std::string get_component_list() override{
            std::string component_list = "{\"" + this->get_name() + "\":[";
            int i = 0;
            for(auto  & component : this->components){
                if(i > 0){
                    component_list += ",";
                }
                component_list += component->get_component_list();
                i++;
            }
            component_list += "]}";
            return component_list;
        }

        void init_components(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components){
            for(auto & component : components){
                this->components.push_back(component);
            }
        }

    protected:
        std::vector<std::shared_ptr<ImgProcessingComponent>> components;
};

#endif // COMPOSIT_COMPONENT_H