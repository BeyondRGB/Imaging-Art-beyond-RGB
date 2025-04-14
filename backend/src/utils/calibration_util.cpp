#include "calibration_util.hpp"
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>

cv::Mat btrgb::calibration::build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count){
    int row_count = targets[0].get_row_count();
    int col_count = targets[0].get_col_count();

    //Calculate row and col count for matrix
    // There should be a row for each channel of each target(ie 2 targes, 3 channels = 6 rows)
    // Each row should hold a number of values equle to the total number of ColorPatches of one ColorTarget
    int mat_col_count = row_count * col_count;
    int mat_row_count = channel_count * target_count;
    std::cout << "Initializing Matrix" << std::endl;
    cv::Mat color_patch_avgs = cv::Mat_<double>(mat_row_count, mat_col_count, CV_32FC1);
    // Iterate over each Target
    std::cout << "Filling Matrix" << std::endl;
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

cv::Mat btrgb::calibration::apply_offsets(cv::Mat camera_sigs, cv::Mat offsets){
    int row_count = camera_sigs.rows;
    int col_count = camera_sigs.cols;
    cv::Mat_<double> res(row_count, col_count, CV_64FC1);
    for(int row = 0; row < row_count; row++){
        double offset = offsets.at<double>(row);
        for(int col = 0; col < col_count; col++){
            double value = camera_sigs.at<double>(row,col);
            res.at<double>(row,col) = value - offset;
        }
    }
    return res;
}

cv::Mat btrgb::calibration::calc_R_camera(cv::Mat M_refl, cv::Mat camera_sigs){
    cv::Mat R_camera = M_refl * camera_sigs;
    for(int row = 0; row < R_camera.rows; row++){
        for(int col = 0; col < R_camera.cols; col++){
            // If any value is found to be negative set it to zero
            if(R_camera.at<double>(row,col) < 0){
                R_camera.at<double>(row,col) = 0;
            }
        }
    }
    return R_camera;
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
        std::cout << "All Values are positive." << std::endl;
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

btrgb::Image* btrgb::calibration::camera_sigs_2_image(cv::Mat camera_sigs, int height){
    btrgb::Image *img = new btrgb::Image("Spectral");
    cv::Mat transpose = camera_sigs.t();
    cv::Mat img_data = transpose.reshape(camera_sigs.rows, height);
    img->initImage(img_data);
    return img;
}

cv::Mat btrgb::calibration::image_2_camera_sigs(btrgb::Image *image, int height, int width){
    cv::Mat img_data = image->getMat();
    img_data = img_data.reshape(1, height * width);
    cv::Mat camera_sigs = img_data.t();
    return camera_sigs;
}

void btrgb::calibration::fill_Lab_values(cv::Mat *L_camera, cv::Mat *a_camera, cv::Mat *b_camera,
                         cv::Mat *L_ref,    cv::Mat *a_ref,    cv::Mat *b_ref,
                         cv::Mat xyz, RefData *ref_data){
    
    int row_count = ref_data->get_row_count();
    int col_count = ref_data->get_col_count();
    *L_camera = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *b_camera = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *a_camera = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *L_ref = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *a_ref = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *b_ref = cv::Mat_<double>(row_count, col_count, CV_64FC1);

    WhitePoints* wp = ref_data->get_white_pts();
    for(int row = 0; row < row_count; row++){
        for(int col = 0; col < col_count; col++){
            // Get/Store L*,a*,b* values from RefData
            L_ref->at<double>(row,col) = ref_data->get_L(row, col);
            a_ref->at<double>(row,col) = ref_data->get_a(row, col);
            b_ref->at<double>(row,col) = ref_data->get_b(row, col);

            // Extract current camera_(x,y,z)
            // Scale each by 100 because everything in xyz is between 0-1 and we need to match the scale of the RefData
            int xyz_index = col + row * col_count;
            double x = 100 * xyz.at<double>(0, xyz_index);
            double y = 100 * xyz.at<double>(1, xyz_index);
            double z = 100 * xyz.at<double>(2, xyz_index);
            // Convert camera_(x,y,z) to camera_(L*,a*,b*)
            btrgb::XYZ_t xyz = {x, y, z};
            btrgb::Lab_t lab = btrgb::xyz_2_Lab(xyz, wp);
            // Stor L*,a*,b* values from camera sigs
            L_camera->at<double>(row,col) = lab.L;
            a_camera->at<double>(row,col) = lab.a;
            b_camera->at<double>(row,col) = lab.b;
        }
    }
}

double btrgb::calibration::compute_deltaE_sum(RefData *ref_data, cv::Mat xyz, cv::Mat *deltaE_values){
    // Establish vars for DeltaE calculation
    int row_count = ref_data->get_row_count();
    int col_count = ref_data->get_col_count();
    double ref_L;
    double ref_a;
    double ref_b;
    double L;
    double a;
    double b;

    // Calculate AVG delta E for all ColorPatches on target
    // delta E is the difference in color between the RefData and the actual image Target(xyz Mat)
    WhitePoints* wp = ref_data->get_white_pts();
    double deltaE_sum = 0;
    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < col_count; col++) {
            // Get L*,a*,b* values from RefData
            ref_L = ref_data->get_L(row, col);
            ref_a = ref_data->get_a(row, col);
            ref_b = ref_data->get_b(row, col);

            // Extract current camera_(x,y,z)
            // Scale each by 100 because everything in xyz is between 0-1 and we need to match the scale of the RefData
            int xyz_index = col + row * col_count;
            double x = 100 * xyz.at<double>(0, xyz_index);
            double y = 100 * xyz.at<double>(1, xyz_index);
            double z = 100 * xyz.at<double>(2, xyz_index);
            // Convert camera_(x,y,z) to camera_(L*,a*,b*)
            btrgb::XYZ_t xyz = {x, y, z};
            btrgb::Lab_t lab = btrgb::xyz_2_Lab(xyz, wp);

            // Calculate deltaE and add to sum
            cmsCIELab lab1 = {ref_L, ref_a, ref_b};
            cmsCIELab lab2 = {lab.L, lab.a, lab.b};
            double delE = cmsCIE2000DeltaE(&lab1, &lab2, 1, 1, 1);
            // Store value in matrix. This matrix will hold the actual deltaE values for each patch for the min avg found
            deltaE_values->at<double>(row,col) = delE;
            deltaE_sum += delE;
        }
    }
    return deltaE_sum;
}

double btrgb::calibration::compute_90th_percentile(const cv::Mat& deltaE_values) {
    std::vector<double> values;
    for (int row = 0; row < deltaE_values.rows; row++) {
        for (int col = 0; col < deltaE_values.cols; col++) {
            values.push_back(deltaE_values.at<double>(row, col));
        }
    }
    if (values.empty()) {
        throw std::runtime_error("DeltaE matrix is empty, cannot compute 90th percentile.");
    }
    std::sort(values.begin(), values.end());
    int index = static_cast<int>(0.9 * values.size());
    if (index >= values.size()) {
        index = values.size() - 1;
    }

    return values[index];
}

//double p90 = btrgb::calibration::compute_90th_percentile(deltaE_values);

double btrgb::calibration::compute_RMSE(cv::Mat R_camera, cv::Mat R_ref){
    double RMSE = 0;
    
    // N should be equivilent to 36 ie. the number of wavelengths ie. the number of rows in R_camera/R_ref 
    int N = R_camera.rows;
    for(int row = 0; row < R_ref.rows; row++){
        for(int col = 0; col < R_ref.cols; col++){
            double camera_val = R_camera.at<double>(row,col);
            double ref_data_value = R_ref.at<double>(row, col);
            RMSE += pow((camera_val - ref_data_value), 2) / N;
        }
    }
    RMSE = sqrt(RMSE);
    return RMSE;
}