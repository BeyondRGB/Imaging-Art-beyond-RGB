#ifndef SPECTRALCALIBRATOR_H
#define SPECTRALCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>

#include "ImageUtil/ColorTarget.hpp"
#include "utils/calibration_util.hpp"
#include "reference_data/ref_data.hpp"
#include "reference_data/ref_data_defines.hpp"

#include "ImgProcessingComponent.h"

class SpectralCalibrator : public ImgProcessingComponent{
public:
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
    void my_callback(std::string str);

private:
    RefData* ref_data;
    cv::Mat color_patch_avgs;
    cv::Mat M_refl;

    void init_M_refl();

};

class WeightedErrorFunction: public cv::MinProblemSolver::Function{

    public:
    WeightedErrorFunction();
    int getDims() const;

    double calc(const double* x) const;
};


#endif //SPECTRALCALIBRATOR_H
