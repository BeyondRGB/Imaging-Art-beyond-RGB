//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_FLATFIELDOR_H
#define BEYOND_RGB_BACKEND_FLATFIELDOR_H


#include "ImgProcessingComponent.h"
#include "ImageUtil/Image.hpp"

class FlatFieldor : public ImgProcessingComponent{
private:
    float w;
    void wCalc(int base, int rings, int patX, int patY, double yRef, btrgb::Image* a1, btrgb::Image* a2, btrgb::Image* wh1, btrgb::Image* wh2);
    void pixelOperation(int h, int w, int c, btrgb::Image* a1, btrgb::Image* a2, btrgb::Image* wh1, btrgb::Image* wh2, btrgb::Image* d1, btrgb::Image* d2);
public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_FLATFEILDOR_H
