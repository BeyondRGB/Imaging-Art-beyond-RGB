#ifndef BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
#define BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>

#include "ImageUtil/ColorTarget.hpp"

#include "ImgProcessingComponent.h"

class ColorManagedCalibrator : public ImgProcessingComponent{
public:
    ~ColorManagedCalibrator();
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);

private:
    cv::Mat* color_patch_avgs;

    ColorTarget get_target(btrgb::ArtObject* images, btrgb::Image* im);
    void build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count);

    void display_avg_matrix();
};


#endif //BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
