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

cv::Mat btrgb::calibration::build_camra_signals_matrix(Image* art[], int art_count, int channel_count, cv::Mat* offsets){
    int height = art[0]->height();
    int width = art[0]->width();
    // Initialize 6xN Matrix to represen our 6 channal image
    // Each row represents a single channel and N is the number total pixles for each channel
    cv::Mat camra_sigs = cv::Mat_<double>(channel_count, height * width, CV_32FC1);
    int chan_count = 3; // Each image only has 3 channels
    for(int art_i = 0; art_i < art_count; art_i++){
        // The art image we are currently getting pixel values from
        btrgb::Image* art_c = art[art_i];
        for(int chan = 0; chan < chan_count; chan++){
            // The row in the six_chan matrix we are currently adding values to
            int mat_row = chan + art_i * chan_count;
            for(int row = 0; row < art_c->height(); row++){
                int offset_index = chan + art_i * 3;
                // The offset value subracted from each pixel, but we only need to subtract offsets if some were given. Offsets default to null
                double offset_value = 0;
                if(nullptr != offsets){
                    offset_value = offsets->at<double>(offset_index);
                }
                for(int col = 0; col < art_c->width(); col++){
                    // The pixel value we are going to set. 
                    // NOTE: this includes the subraction of the offset_value
                    double px_val = (double)art_c->getPixel(row, col, chan) - offset_value;
                    // The col in the six_chan matrix we ar currently adding values to
                    int mat_col = col + row * art_c->width();
                    // Set pixel
                    camra_sigs.at<double>(mat_row, mat_col) = px_val;
                }
            }
        }
    }
    return camra_sigs;
}

void btrgb::calibration::display_matrix(cv::Mat* matrix, std::string name) {
    bool contains_negatives = false;
    std::cout << std::endl;
    std::cout << "What is in " << name << std::endl;
    if (nullptr != matrix) {
        for (int chan = 0; chan < matrix->rows; chan++) {
            for (int col = 0; col < matrix->cols; col++) {
                if (col != 0) {
                    std::cout << ", ";
                }
                double val = matrix->at<double>(chan, col);
                std::cout << val;
                if(val < 0)
                    contains_negatives = true;
            }
            std::cout << std::endl;// << std::endl;
        }
    }
    else {
        std::cout << "Matrix not initialized" << std::endl;
    }
    
    std::cout << name << " rows: " << matrix->rows << " " << name << " cols: " << matrix->cols << std::endl;
    if(contains_negatives)
        std::cout << "Contains Negative Values." << std::endl;
    else
        std::cout << "All Values are posotive." << std::endl;
}

double btrgb::calibration::row_min(cv::Mat &target, int row){
    double min = std::numeric_limits<double>::max();//MAX;
    for(int col = 0; col < target.cols; col++){
        double val = target.at<double>(row, col);
        if(val < min){
            min = val;
        }
    }
    return min;
}

double btrgb::calibration::row_max(cv::Mat &target, int row){
    double max = std::numeric_limits<double>::min();//MIN;
    for(int col = 0; col < target.cols; col++){
        double val = target.at<double>(row, col);
        if(val > max){
            max = val;
        }
    }
    return max;
}

void btrgb::calibration::enter_to_continue(){
        std::cout << "Enter To Continue.";
        char c;
        do{
            std::cin >> std::noskipws >> c;
            std::cout << "c: (" << c << ")" << std::endl;
        }while(c != '\n');
    }