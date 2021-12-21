//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_BITDEPTHSCALOR_H
#define BEYOND_RGB_BACKEND_BITDEPTHSCALOR_H


#include "ImgProcessingComponent.h"

class BitDepthScalor : public ImgProcessingComponent {

public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_BITDEPTHSCALOR_H
