#ifndef COLORIMETRY_H
#define COLORIMETRY_H

#include "image_processing/results/results_formater.hpp"

class ColorimetryFormater : public ResultsFormater{
    private:
        std::string colum_headers[20] = {
            "Patch ID",
            "DE00",
            "L_ref",
            "a_ref",
            "b_ref",
            "L_camera",
            "a_camera",
            "b_camera",
            "X_ref",
            "Y_ref",
            "Z_ref",
            "X_camera",
            "Y_camera",
            "Z_camera",
            "R_1_camera",
            "G_1_camera",
            "B_1_camera",
            "R_2_camera",
            "G_2_camera",
            "B_2_camera"
        };
        void write_header(std::ostream &output_stream, std::string delim);

    public:
        void write_format(std::ostream &output_stream, CalibrationResults *results) override;

};

#endif // COLORIMETRY_H