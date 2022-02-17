#ifndef BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
#define BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>

#include "ImageUtil/ColorTarget.hpp"

#include "ImgProcessingComponent.h"

class ColorManagedCalibrator : public ImgProcessingComponent{
public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);

private:
    ColorTarget get_target(btrgb::ArtObject* images, btrgb::Image* im);
};


#endif //BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
