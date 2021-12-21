//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_SPECTRALCALIBRATOR_H
#define BEYOND_RGB_BACKEND_SPECTRALCALIBRATOR_H


#include "ImgProcessingComponent.h"

class SpectralCalibrator : public ImgProcessingComponent{
public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);
};


#endif //BEYOND_RGB_BACKEND_SPECTRALCALIBRATOR_H
