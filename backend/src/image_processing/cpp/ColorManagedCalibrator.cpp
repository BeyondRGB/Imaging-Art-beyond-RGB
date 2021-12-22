//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/ColorManagedCalibrator.h"

void ColorManagedCalibrator::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Color Managed Calibration");
    //TODO add Color Manged Calibration logic here
    sleep_for(seconds(1));
}