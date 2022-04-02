#ifndef BEYOND_RGB_BACKEND_CHANNELSELECTOR_H
#define BEYOND_RGB_BACKEND_CHANNELSELECTOR_H


// #include "ImgProcessingComponent.h"
#include "image_processing/header/LeafComponent.h"

class ChannelSelector : public LeafComponent{
public:
    ChannelSelector() : LeafComponent("ChannelSelector"){}
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    // std::string get_component_list() override;
};


#endif //BEYOND_RGB_BACKEND_CHANNELSELECTOR_H
