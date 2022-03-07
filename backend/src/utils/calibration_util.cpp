#include "calibration_util.hpp"

cv::Mat btrgb::calibration::build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count){
    int row_count = targets[0].get_row_count();
    int col_count = targets[0].get_col_count();

    //Calculate row and col count for matrix
    // There should be a row for each channel of each target(ie 2 targes, 3 channels = 6 rows)
    // Each row should hold a number of values equle to the total number of ColorPatches of one ColorTarget
    int mat_col_count = row_count * col_count;
    int mat_row_count = channel_count * target_count;

    cv::Mat color_patch_avgs = cv::Mat_<double>(mat_row_count, mat_col_count, CV_32FC1);
    // Iterate over each Target
    for (int target_i = 0; target_i < target_count; target_i++) {
        ColorTarget target = targets[target_i];
        // For each Target visit each Channel
        for (int chan = 0; chan < channel_count; chan++) {
            // Calculate the row of matrex to put data
            int mat_row = chan + target_i * channel_count;
            // Iterate over all ColorPatches for current Target
            for (int target_row = 0; target_row < row_count; target_row++) {
                for (int target_col = 0; target_col < col_count; target_col++) {
                    // Calculate the colum of matrix to put data
                    int mat_col = target_col + target_row * col_count;
                    // Get avg pixel color for current ColorPatch
                    float avg = target.get_patch_avg(target_row, target_col, chan);
                    // Stroe avg value
                    color_patch_avgs.at<double>(mat_row, mat_col) = (double)avg;
                }
            }
        }
    }
    return color_patch_avgs;

}