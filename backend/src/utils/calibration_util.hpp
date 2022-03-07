#ifndef CALIBRATION_UTIL_H
#define CALIBRATION_UTIL_H


#include <opencv2/opencv.hpp>

#include "ImageUtil/ColorTarget.hpp"

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
    }
}

#endif // CALIBRATION_UTIL_H