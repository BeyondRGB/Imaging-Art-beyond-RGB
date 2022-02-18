#ifndef BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
#define BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>

#include "ImageUtil/ColorTarget.hpp"
#include "utils/csv_parser.hpp"
#include "ImgProcessingComponent.h"
#include "reference_data/ref_data_defines.hpp"

class ColorManagedCalibrator : public ImgProcessingComponent{
public:
    ~ColorManagedCalibrator();
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);


    void build_input_matrix();
    void display_avg_matrix(cv::Mat* matrix);

private:
    cv::Mat* color_patch_avgs = nullptr;
    cv::Mat optimization_input;
    cv::Mat M;
    cv::Mat offest;


    ColorTarget get_target(btrgb::ArtObject* images, btrgb::Image* im);
    void build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count);
    
    //void build_input_matrix();
    //void display_avg_matrix(cv::Mat* matrix);
};


#endif //BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
