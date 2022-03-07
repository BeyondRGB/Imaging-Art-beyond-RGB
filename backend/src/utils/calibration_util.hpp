#ifndef CALIBRATION_UTIL_H
#define CALIBRATION_UTIL_H


#include <opencv2/opencv.hpp>

#include "ImageUtil/ColorTarget.hpp"
#include "ImageUtil/Image.hpp"

namespace btrgb{
    namespace calibration{
        /**
         * @brief Initialize a MxN matrix to hold the average pixel values for each color patch where 
         * M is the number of channels
         * N is the number of Color Patches 
         * 
         *   color_patch_avg is a 2d Matrix in the form 
         *   (cp_avg is the average pixel value from the color target in the actual image)
         *       cp_avg_chan1_patch_1, cp_avg_chan1_patch_2, ..., cp_avg_chan1_patch_k
         *       cp_avg_chan2_patch_1, cp_avg_chan2_patch_2, ..., cp_avg_chan2_patch_k
         *       ...                 , ...                 , ..., ...
         *       cp_avg_chan6_patch_1, cp_avg_chan6_patch_2, ..., cp_avg_chan6_patch_k 
         * @param targets an array of color targets
         * @param target_count the number of color targets
         * @param channel_count the number of channels (M) this Matrix will hold
         */
        cv::Mat build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count);

        /**
         * @brief Initialize a MxN matrix to hold all camra signal pixel values where
         * M is the number of channels
         * N is the number of pixels per channel( ie. width x height of image)
         * 
         *  camsigs is a 2d Matrix in the form
         *      camsigs_chan1_px1 - offset1, camsigs_chan1_px2 - offset1, ..., camsigs_chan1_pxN - offset1
         *      camsigs_chan2_px1 - offset2, camsigs_chan2_px2 - offset2, ..., camsigs_chan2_pxN - offset2
         *      ...                 , ...                 , ..., ...
         *      camsigs_chanM_px1 - offsetM, camsigs_chanM_px2 - offsetM, ..., camsigs_chanM_pxN - offsetM
         * 
         * @param images an array of art Images to get pixel values from
         * @param art_count the number of art images provided
         * @param channel_count M
         * @param offsets optional offset values, if provided this should be a 1XM matrix where M is the number of channels of camrasigs. This defults to nullptr.
         * @return cv::Mat camrasigs
         */
        cv::Mat build_camra_signals_matrix(Image* images[], int art_count, int channel_count, cv::Mat* offsets=nullptr);
    
        /**
         * @brief Helper function for displaying a matrix to stdout
         * 
         * @param matrix the matrix to be displayed
         * @param name the name of the matrix that will be displyed along with values
         */
        void display_matrix(cv::Mat* matrix, std::string name);
    }
}

#endif // CALIBRATION_UTIL_H