//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_FLATFEILDOR_H
#define BEYOND_RGB_BACKEND_FLATFEILDOR_H


#include "ImgProcessingComponent.h"

class FlatFeildor : public ImgProcessingComponent{
public:
    void execute(callback func) override;
};


#endif //BEYOND_RGB_BACKEND_FLATFEILDOR_H
