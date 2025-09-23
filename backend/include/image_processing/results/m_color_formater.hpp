#ifndef M_COLOR_FORMATER_H
#define M_COLOR_FORMATER_H

#include "image_processing/results/results_formater.hpp"

class MColorFormater : public ResultsFormater{

    private:
    
    public:
        void write_format(std::ostream &output_stream, CalibrationResults *results, ResultObjType format_type) override;
    
};

#endif // M_COLOR_FORMATER_H