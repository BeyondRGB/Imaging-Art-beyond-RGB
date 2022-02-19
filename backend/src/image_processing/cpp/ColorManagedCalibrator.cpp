

#include "../header/ColorManagedCalibrator.h"

ColorManagedCalibrator::~ColorManagedCalibrator() {
}

void ColorManagedCalibrator::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Color Managed Calibration");
    
    btrgb::Image* art1;
    btrgb::Image* art2;
    ColorTarget target1;
    ColorTarget target2;

    std::cout << "\n\nStarting Tests ColorMangedCalibrator\n\n";

    try {
        art1 = images->getImage("art1");
        art2 = images->getImage("art2");
        target1 = this->get_target(images, art1);
        target2 = this->get_target(images, art2);
        this->ref_data = images->get_refrence_data();
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        func("Error: Flatfielding called out of order. Missing at least 1 image assignment.");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        func("Error: " + error);
        return;
    }
    ColorTarget targets[] = { target1, target2 };
    int channel_count = art1->channels();
    int target_count = std::size(targets);

    this->build_target_avg_matrix(targets, target_count, channel_count);
    this->display_matrix(&this->color_patch_avgs);

    this->build_input_matrix();

    this->find_optimization();
    


    sleep_for(seconds(5));
}

void ColorManagedCalibrator::find_optimization() {
    double dE = this->compute_deltaE(this->color_patch_avgs);
    std::cout << "deltaE: " << dE << std::endl;
}

double ColorManagedCalibrator::compute_deltaE(cv::Mat input) {
    int row_count = this->color_patch_avgs.rows;
    int col_count = this->color_patch_avgs.cols;
    cv::Mat offset_avg(row_count, col_count, CV_32FC1);
    for (int row = 0; row < row_count; row++) {
        float offset = this->offest.at<float>(row);
        for (int col = 0; col < col_count; col++) {
            float avg = this->color_patch_avgs.at<float>(row, col);
            offset_avg.at<float>(row, col) = avg - offset;
        }
    }
    cv::Mat xyz = this->M * offset_avg;

    row_count = this->ref_data->get_row_count();
    col_count = this->ref_data->get_col_count();
    double ref_L;
    double ref_a;
    double ref_b;
    double L;
    double a;
    double b;
    
    // Calculate AVG delta E for all ColorPatches on target
    // delta E is the difference in color between the RefData and the actual image Target(xyz Mat)
    WhitePoints* wp = this->ref_data->get_white_pts();
    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < col_count; col++) {
            ref_L = this->ref_data->get_L(row, col);
            ref_a = this->ref_data->get_a(row, col);
            ref_b = this->ref_data->get_b(row, col);

            int xyz_index = col + row * col_count;
            double x = (double)xyz.at<float>(0, xyz_index);
            double y = (double)xyz.at<float>(1, xyz_index);
            double z = (double)xyz.at<float>(2, xyz_index);

            btrgb::XYZ_t xyz = {x, y, z};
            btrgb::Lab_t lab = btrgb::xyz_2_Lab(xyz, wp);

            cmsCIELab lab1(ref_L, ref_a, ref_b);
            cmsCIELab lab2(lab.L, lab.a, lab.b);
            double deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1, 1, 1);
            return deltaE;
        }
    }
    

    return 5;
}


ColorTarget ColorManagedCalibrator::get_target(btrgb::ArtObject* images, btrgb::Image* im) {
    //double target_top_location;
    //double target_bottom_location;
    //double target_left_location;
    //double target_right_location;
    ////int target_width;
    ////int target_height;
    //int target_row_count;
    //int target_col_count;
    TargetData target_data;

    try {
        target_data.top_loc = images->getTargetInfo("top");
        target_data.bot_loc = images->getTargetInfo("bot");
        target_data.left_loc = images->getTargetInfo("left");
        target_data.right_loc = images->getTargetInfo("right");

        target_data.row_count = images->getTargetSize("row");
        target_data.col_count = images->getTargetSize("col");
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        throw e;
    }
    catch (const std::logic_error& e) {
        throw e;
    }
    
  
    ColorTarget target(im, target_data);
   
    return target;
}

void ColorManagedCalibrator::build_target_avg_matrix(ColorTarget targets[], int target_count, int channel_count) {
    int row_count = targets[0].get_row_count();
    int col_count = targets[0].get_col_count();

    //Calculate row and col count for matrix
    // There should be a row for each channel of each target(ie 2 targes, 3 channels = 6 rows)
    // Each row should hold a number of values equle to the total number of ColorPatches of one ColorTarget
    int mat_col_count = row_count * col_count;
    int mat_row_count = channel_count * target_count;

    this->color_patch_avgs = cv::Mat(mat_row_count, mat_col_count, CV_32FC1);
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
                    this->color_patch_avgs.at<float>(mat_row, mat_col) = avg;
                    std::cout << avg << ",";
                }
            }
            std::cout << std::endl << std::endl;
        }
    }
}

void ColorManagedCalibrator::build_input_matrix() {
    CSVParser parser;
    std::string file_path = RES_PATH "calibration_input_matrix.csv";
    parser.open_file(file_path);
    int row_count = parser.get_line_count()-1;
    std::string header = parser.get_next_line();
    int col_count = parser.count_line_items(header)-1;
    this->optimization_input = cv::Mat(row_count, col_count, CV_32FC1);
    int row = 0;
    std::cout << "matRow: " << this->optimization_input.rows << " matCol: " << this->optimization_input.cols << std::endl;
    while (parser.has_next_line()) {
        int col = 0;
        std::string line = parser.get_next_line();
        std::cout << line << std::endl;
        //Ignore the first item in line
        parser.get_next<std::string>(line);
        while (parser.has_next(line)) {
            float value = 0;
            try {
                value = parser.get_next<float>(line);
            }
            catch (boost::wrapexcept<boost::bad_lexical_cast> e) {
                std::cerr << "Bad value found in csv file (row: " << row+1 << " col: " << col+1 << std::endl;
            }
            std::cout << "value: " << value << " row: " << row << " col: " << col << std::endl;
            this->optimization_input.at<float>(row, col++) = value;
        }
        row++;
    }
    this->M = this->optimization_input(cv::Rect(0, 0, col_count, row_count - 1));
    this->offest = this->optimization_input(cv::Rect(0, row_count - 1, col_count, 1));

    std::cout << "row_count: " << row_count << " col_count: " << col_count << std::endl;
    
    std::cout << "Full Matrix" << std::endl;
    this->display_matrix(&this->optimization_input);
    std::cout << "M Matrix" << std::endl;
    this->display_matrix(&this->M);
    std::cout << "Offset Matrix" << std::endl;
    this->display_matrix(&this->offest);
    parser.close_file();
}

void ColorManagedCalibrator::display_matrix(cv::Mat* matrix) {
    std::cout << std::endl;
    std::cout << "What is in the Mat" << std::endl;
    if (nullptr != matrix) {
        for (int chan = 0; chan < matrix->rows; chan++) {
            for (int col = 0; col < matrix->cols; col++) {
                if (col != 0) {
                    std::cout << ", ";
                }
                float avg = matrix->at<float>(chan, col);
                std::cout << avg;
            }
            std::cout << std::endl << std::endl;
        }
    }
    else {
        std::cout << "Matrix not initialized" << std::endl;
    }
}