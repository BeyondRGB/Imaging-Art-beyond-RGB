//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/SpectralCalibrator.h"

void SpectralCalibrator::execute(CallBackFunction func) {
    func("SpectralCalibration");
    //TODO add Spectral Calibration logic here
    sleep_for(seconds(5));
}