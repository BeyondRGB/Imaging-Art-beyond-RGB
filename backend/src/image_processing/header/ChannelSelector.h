#ifndef BEYOND_RGB_BACKEND_CHANNELSELECTOR_H
#define BEYOND_RGB_BACKEND_CHANNELSELECTOR_H

#include "image_processing/header/LeafComponent.h"

class ChannelSelector : public LeafComponent{
    
public:
    ChannelSelector() : LeafComponent("Channel Selector"){}
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

};

#endif //BEYOND_RGB_BACKEND_CHANNELSELECTOR_H
