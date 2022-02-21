#ifndef BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
#define BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>
#include <lcms2.h>

#include "ImageUtil/ColorTarget.hpp"
#include "utils/csv_parser.hpp"
#include "ImgProcessingComponent.h"
#include "reference_data/ref_data_defines.hpp"
#include "reference_data/ref_data.hpp"
#include "utils/color_convertions.hpp"
#include "reference_data/white_points.hpp"

//#include <ceres.h>

//typedef double (*minimize_detlaE)(cv::Mat);
typedef std::function<double(cv::Mat)> MinDeltaE_function;

class ColorManagedCalibrator : public ImgProcessingComponent{

    

public:
    ~ColorManagedCalibrator();
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);

    void build_input_matrix();
    void display_matrix(cv::Mat* matrix, std::string name);

private:
    cv::Mat color_patch_avgs;
    cv::Mat optimization_input;
    cv::Mat M;
    cv::Mat offest;
    RefData* ref_data;



    ColorTarget get_target(btrgb::ArtObject* images, btrgb::Image* im);
    void build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count);
    void find_optimization();
    
    //void build_input_matrix();
    //void display_avg_matrix(cv::Mat* matrix);

    
};

class DeltaEFunction: public cv::MinProblemSolver::Function{
public: 
    DeltaEFunction(cv::Mat* opt_in, cv::Mat* cp_avgs, cv::Mat* offeset, cv::Mat* M, RefData* ref_data);
    int getDims() const;
    double calc(const double* x)const;
    //void getGradient(const double* x, double* grad);
    void crop_input(const double* x);
private:
    cv::Mat* opt_in;
    cv::Mat* offeset;
    cv::Mat* M;
    cv::Mat* color_patch_avgs = nullptr;
    RefData* ref_data = nullptr;
};


#endif //BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
