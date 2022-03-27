#ifndef CALIBRATION_UTIL_H
#define CALIBRATION_UTIL_H


#include <opencv2/opencv.hpp>
#include <limits>

#include "ImageUtil/ColorTarget.hpp"
#include "ImageUtil/Image.hpp"

// #define MAX std::numeric_limits<double>::max()
// #define MIN std::numeric_limits<double>::min()

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
         * @brief Create a matrix with the given offsets applied to the values of given camera sigs
         * 
         *  camsigs is a 2d Matrix in the form
         *      camsigs_chan1_px1, camsigs_chan1_px2, ..., camsigs_chan1_pxN
         *      camsigs_chan2_px1, camsigs_chan2_px2, ..., camsigs_chan2_pxN
         *      ...                 , ...                 , ..., ...
         *      camsigs_chanM_px1, camsigs_chanM_px2, ..., camsigs_chanM_pxN
         * 
         *  offsets is a 1d matrix in the form
         *      offset1, offset2, ... offsetM
         * 
         *  result is a 2d Matrix in the form
         *      camsigs_chan1_px1 - offset1, camsigs_chan1_px2 - offset1, ..., camsigs_chan1_pxN - offset1
         *      camsigs_chan2_px1 - offset2, camsigs_chan2_px2 - offset2, ..., camsigs_chan2_pxN - offset2
         *      ...                 , ...                 , ..., ...
         *      camsigs_chanM_px1 - offsetM, camsigs_chanM_px2 - offsetM, ..., camsigs_chanM_pxN - offsetM
         * 
         * @param camrera_sigs the values to apply the offset to
         * @param offsets the offset values to apply
         * @return cv::Mat the resulting matrix 
         */
        cv::Mat apply_offsets(cv::Mat camera_sigs, cv::Mat offsets);

        /**
         * @brief Calculats R_camera and clips any value < 0 to ensure that all values are posotive
         * 
         *   M_refl is a 2d Matrix in the form
         *      m_1_1,  m_1_2,  ..., m_1_6
         *      m_2_1,  m_2_2,  ..., m_2_6
         *      ...  ,  ...  ,  ..., ...
         *      m_36_1, m_36_2, ..., m_36_6
         * 
         *   camera_sigs is a 2d Matrix in the form
         *      camsigs_chan1_px1, camsigs_chan1_px2, ..., camsigs_chan1_pxN
         *      camsigs_chan2_px1, camsigs_chan2_px2, ..., camsigs_chan2_pxN
         *      ...                 , ...                 , ..., ...
         *      camsigs_chanM_px1, camsigs_chanM_px2, ..., camsigs_chanM_pxN
         * 
         *  R_camera is a 2d Matrix in fthe form
         *      RLamda_1_1,  RLamda_1_2,  ..., RLamda_1_k
         *      RLamda_2_1,  RLamda_2_2,  ..., RLamda_2_k
         *      ...       ,  ...       ,  ..., ...
         *      RLamda_36_1, RLamda_36_2, ..., RLamda_36_k
         * 
         * @param M_refl Optimized matrix found during Spectral Calibration and stored in Results
         * @param camera_sigs the actual pixel values from the camera weather its just from a color target or the entrier image
         * @return cv::Mat R_camera
         */
        cv::Mat calc_R_camera(cv::Mat M_refl, cv::Mat camera_sigs);
    
        /**
         * @brief Helper function for displaying a matrix to stdout
         * 
         * @param matrix the matrix to be displayed
         * @param name the name of the matrix that will be displyed along with values
         */
        void display_matrix(cv::Mat* matrix, std::string name);

        /**
         * @brief Find min value in specified targets row 
         * 
         * @param target: the target matrix to look for min value
         * @param row: the row to look for min in target
         * @return double min value
         */
        double row_min(cv::Mat &target, int row);

        /**
         * @brief Find the max value in specified tartets row
         * 
         * @param target the target matrix to look for max value
         * @param row the row to look for max in target
         * @return double max value
         */
        double row_max(cv::Mat &target, int row);

        /**
         * @brief Debuging helper functon. Will puase and wait for user to hit enter in terminal befor proceding
         * 
         */
        void enter_to_continue();

        btrgb::Image* camera_sigs_2_image(cv::Mat camera_sigs, int height);
        cv::Mat image_2_camera_sigs(btrgb::Image *image, int height, int width);

    }
}

#endif // CALIBRATION_UTIL_H