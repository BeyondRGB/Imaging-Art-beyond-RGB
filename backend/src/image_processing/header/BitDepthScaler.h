#ifndef BEYOND_RGB_BACKEND_BITDEPTHSCALER_H
#define BEYOND_RGB_BACKEND_BITDEPTHSCALER_H


#include "ImgProcessingComponent.h"
#include "image_processing/header/BitDepthScalerStrategy.h"

class BitDepthScaler : public ImgProcessingComponent {

    public:
        BitDepthScaler();
        BitDepthScaler(std::string strategy);
        ~BitDepthScaler();
        void execute(CallBackFunction func, btrgb::ArtObject* images) override;

    private:
        BitDepthScalerStrategy* strategy;
};


#endif
