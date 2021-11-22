//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ChannelSelector.h"

void ChannelSelector::execute(CallBackFunction func, ArtObject* images) {
    func("Channel Selection");
    //TODO add channel section logic here
    sleep_for(seconds(5));
}