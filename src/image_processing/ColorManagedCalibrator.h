//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
#define BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H


#include "header/ImgProcessingComponent.h"

class ColorManagedCalibrator : public ImgProcessingComponent{
public:
    void execute(CallBackFunction func) override;
    void my_callback(string str);
};


#endif //BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
