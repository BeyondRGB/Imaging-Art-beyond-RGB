//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_FLATFIELDOR_H
#define BEYOND_RGB_BACKEND_FLATFIELDOR_H


#include "ImgProcessingComponent.h"

class FlatFieldor : public ImgProcessingComponent{
private:
    float w1, w2;
    void wCalc(int base, int rings, int patX, int patY, double yRef, btrgb::image* a1, btrgb::image* a2, btrgb::image* wh1, btrgb::image* wh2);
    void pixelOperation(int h, int w, int c, btrgb::image* a1, btrgb::image* a2, btrgb::image* wh1, btrgb::image* wh2, btrgb::image* d1, btrgb::image* d2);
public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_FLATFEILDOR_H
