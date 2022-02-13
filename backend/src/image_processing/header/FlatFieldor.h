//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_FLATFIELDOR_H
#define BEYOND_RGB_BACKEND_FLATFIELDOR_H


#include "ImgProcessingComponent.h"

class FlatFieldor : public ImgProcessingComponent{
public:
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_FLATFEILDOR_H
