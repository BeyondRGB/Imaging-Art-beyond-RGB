//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ChannelSelector.h"

void ChannelSelector::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("", this->get_name());
    //TODO add channel section logic here
    //sleep_for(seconds(1));
}

