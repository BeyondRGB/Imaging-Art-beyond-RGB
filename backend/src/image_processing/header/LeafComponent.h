#ifndef LEAF_COMPONENT_H
#define LEAF_COMPONENT_H

#include "image_processing/header/ImgProcessingComponent.h"

class LeafComponent : public ImgProcessingComponent{

    public:
    LeafComponent(std::string name) : ImgProcessingComponent(name){}
    std::string get_component_list() override{
        return "{\"" + this->get_name() + "\":[]}";
    }

};

#endif // LEAF_COMPONENT_H