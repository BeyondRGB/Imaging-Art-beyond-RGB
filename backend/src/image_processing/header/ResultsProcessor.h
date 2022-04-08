#ifndef RESULTS_PROCESSOR_H
#define RESULTS_PROCESSOR_H

#include "ImgProcessingComponent.h"
#include "image_processing/results/calibration_results.hpp"
#include "image_processing/results/results_formater.hpp"
#include "image_processing/results/general_info_formater.hpp"
#include "image_processing/results/m_color_formater.hpp"
#include "image_processing/results/m_spectral_formater.hpp"
#include "utils/general_utils.hpp"

#define IMG_FILE_NAME(x, id) ("BTRGB_" x "_" id)

/**
 * @brief This class is one of the ImagProcessingComponents and is the last step in the pipeline
 * It is responsible for outputing results of the calibration process including outputing the final image and any/all results reported
 * 
 */
class ResultsProcessor : public ImgProcessingComponent{
    
    public:
        enum FormatType{
            GEN_INFO,
            M_COLOR,
            M_SPECTRAL,
            R_REF,
            COLORIMETRY,
            R_CAMERA
        };
    
        ResultsProcessor(){};
        ~ResultsProcessor();
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

    private:
        /**
         * @brief This will output all results to user readable .txt and .csv files
         * 
         * @param images the ArtObject containing the results
         */
        void output_user_results(btrgb::ArtObject* images);

        /**
         * @brief This outputs all results to a results.btrgb in a json format.
         * This fill contains any and all results from the entire process and the resulting file can be used
         * to recreate the CalibrationResult objects
         * 
         * @param images the ArtObject containing the results
         */
        void output_btrgb_results(btrgb::ArtObject* images);

        /**
         * @brief This will write any image file still remaining in the ArtObject to a .TIFF
         * 
         * @param images the ArtObject containing the images
         */
        void output_images(btrgb::ArtObject* images);

        std::string build_output_name(std::string name, std::string extention="");

        void set_formater(FormatType type);

        ResultsFormater *formater = nullptr;

        std::string output_dir;
        std::string ts_id;

        std::string CM_f_name;
        std::string SP_f_name;
        std::string Pro_f_name;
        // std::string CalibRes_f_name;
        // std::string VerRes_f_name;

        std::string GI_f_name;
        std::string M_color_f_name;
        std::string M_spectral_f_name;
        std::string R_ref_f_name;
        std::string colorimetry_f_name;
        std::string R_camera_f_name;

};

#endif //RESULTS_PROCESSOR_H