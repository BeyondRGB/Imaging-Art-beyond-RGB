#ifndef BEYOND_RGB_BACKEND_BITDEPTHSCALER_H
#define BEYOND_RGB_BACKEND_BITDEPTHSCALER_H


#include "ImgProcessingComponent.h"

class BitDepthScaler : public ImgProcessingComponent {

    public:
        BitDepthScaler();
        ~BitDepthScaler();
        void execute(CallBackFunction func, btrgb::ArtObject* images) override;
};


#endif
