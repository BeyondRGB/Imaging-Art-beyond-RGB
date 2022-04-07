#ifndef RESULTS_FORMATER_H
#define RESULTS_FORMATER_H

#include "image_processing/results/calibration_results.hpp"

#include <iostream>
#include <fstream>

class ResultsFormater{

    public:
    virtual void write_format(std::ostream &output_stream, CalibrationResults *results) = 0;

};

#endif // RESULTS_FORMATER_H