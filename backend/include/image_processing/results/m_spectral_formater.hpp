#ifndef M_SPECTRAL_FORMATER_H
#define M_SPECTRAL_FORMATER_H

#include "image_processing/results/results_formater.hpp"

class MSpectralFormater : public ResultsFormater {

  private:
  public:
    void write_format(std::ostream &output_stream, CalibrationResults *results,
                      ResultObjType format_type) override;
};

#endif // M_SPECTRAL_FORMATER_H