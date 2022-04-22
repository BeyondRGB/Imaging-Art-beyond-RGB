#ifndef RESULTS_FORMATER_H
#define RESULTS_FORMATER_H

#include "image_processing/results/calibration_results.hpp"

#include <iostream>
#include <fstream>

class ResultsFormater{

    public:
    enum ResultObjType{
        GENERAL, CALIBRATION, VERIFICATION
    };

    virtual ~ResultsFormater() {}
    virtual void write_format(std::ostream &output_stream, CalibrationResults *results, ResultObjType format_type) = 0;

};

#endif // RESULTS_FORMATER_H
