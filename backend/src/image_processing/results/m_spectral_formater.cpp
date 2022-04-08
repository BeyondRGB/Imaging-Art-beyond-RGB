#include "image_processing/results/m_spectral_formater.hpp"
#include <opencv2/opencv.hpp>

void MSpectralFormater::write_format(std::ostream &output_stream, CalibrationResults *results){
    cv::Mat M;
    try{
        M = results->get_matrix(SP_M_refl);
        results->write_matrix(output_stream, M, "M_spectral");
    }catch(ParsingError e){ 
        output_stream << e.what() << std::endl; 
    }
}