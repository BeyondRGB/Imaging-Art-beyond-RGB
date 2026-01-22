#ifndef R_CAMERA_FORMATER_H
#define R_CAMERA_FORMATER_H

#include "image_processing/results/results_formater.hpp"
#include "reference_data/ref_data_defines.hpp"

class RCameraFormater : public ResultsFormater {

  private:
    void write_header(std::ostream &output_stream, int row_count, int col_count,
                      std::string delim);
    void write_matrix(std::ostream &output_stream, cv::Mat matrix,
                      int row_count, int col_count, std::string delim);

  public:
    void write_format(std::ostream &output_stream, CalibrationResults *results,
                      ResultObjType format_type) override;
};

#endif // R_CAMERA_FORMATER_H