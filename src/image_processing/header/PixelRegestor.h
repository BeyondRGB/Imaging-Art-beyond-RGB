//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H


#include "ImgProcessingComponent.h"

class PixelRegestor : public ImgProcessingComponent{
public:
    void execute(callback func) override;
};


#endif //BEYOND_RGB_BACKEND_PIXELREGESTOR_H
