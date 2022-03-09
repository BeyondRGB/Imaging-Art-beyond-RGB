#ifndef SPECTRALCALIBRATOR_H
#define SPECTRALCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>

#include "ImageUtil/ColorTarget.hpp"
#include "utils/calibration_util.hpp"
#include "reference_data/ref_data.hpp"
#include "reference_data/ref_data_defines.hpp"
#include "utils/time_tracker.hpp"

#include "ImgProcessingComponent.h"

class SpectralCalibrator : public ImgProcessingComponent{
public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);

private:
    RefData* ref_data;
    cv::Mat color_patch_avgs;
    cv::Mat input_array;
    cv::Mat M_refl;
    cv::Mat R_camera;

    void init_M_refl();
    void init_step(double stp_value, cv::Mat &step);

};


////////////////////////////////////////////////////////////////
//                      WeightedErrorFunction                 //
////////////////////////////////////////////////////////////////

class WeightedErrorFunction: public cv::MinProblemSolver::Function{

public:
    WeightedErrorFunction(cv::Mat *ref_data, cv::Mat *input_array, cv::Mat *M_refl, cv::Mat *cp_carmera_sigs, cv::Mat *R_camera);
    int getDims() const;

    double calc(const double* x) const;

    int get_itteration_count(){return this->itteration_count; }

private:
    static int itteration_count;
    cv::Mat *ref_data;
    cv::Mat *input_array;
    cv::Mat *M_refl;
    cv::Mat *cp_carmera_sigs;
    cv::Mat *R_camera;

    double calc_e1()const;
    double calc_e2()const;
    double calc_e3()const;
    double calc_z(double e1, double e2, double e3)const;
};


#endif //SPECTRALCALIBRATOR_H
