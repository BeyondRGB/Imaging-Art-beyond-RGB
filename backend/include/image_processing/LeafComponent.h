#ifndef LEAF_COMPONENT_H
#define LEAF_COMPONENT_H

#include <image_processing/ImgProcessingComponent.h>

class LeafComponent : public ImgProcessingComponent{

    public:
    LeafComponent(std::string name) : ImgProcessingComponent(name){}
    jsoncons::json get_component_list() override{
        jsoncons::json json;
        json.insert_or_assign("name", this->get_name());
        return json;
    }

};

#endif // LEAF_COMPONENT_H