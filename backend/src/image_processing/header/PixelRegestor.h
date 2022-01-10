
#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H


#include "ImgProcessingComponent.h"

class PixelRegestor : public ImgProcessingComponent{
public:
    void execute(CallBackFunction func) override;
};


#endif //BEYOND_RGB_BACKEND_PIXELREGESTOR_H
