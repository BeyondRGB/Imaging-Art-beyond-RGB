#include <image_processing/ColorManagedCalibrator.h>
#include <filesystem>
#include <utils/calibration_util.hpp>

ColorManagedCalibrator::~ColorManagedCalibrator() {
}

void ColorManagedCalibrator::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    btrgb::Image* art1;
    btrgb::Image* art2;
    ColorTarget target1;
    ColorTarget target2;

    // Set to default for now.
    // This should be updated at some point to be settable by user
    this->color_space = btrgb::ColorSpace::ProPhoto;

    // Extract images and RefData from art object
    try {
        art1 = images->getImage(ART(1));
        art2 = images->getImage(ART(2));
        this->ref_data = images->get_refrence_data();
    }
    catch (const std::exception& e) {
        throw ImgProcessingComponent::error(e.what(), this->get_name());
    }

    // Init Color Targets
    std::unique_ptr<ColorTarget> target1_ptr = images->get_target_pointer(TARGET(1), btrgb::TargetType::GENERAL_TARGET);
    std::unique_ptr<ColorTarget> target2_ptr = images->get_target_pointer(TARGET(2), btrgb::TargetType::GENERAL_TARGET);

    bool targetsFound = (target1_ptr != nullptr && target2_ptr != nullptr);

    if (targetsFound or images->get_batch() == false) {
        //If targets are found, this is an initial request, so full optimization process is done
        target1 = images->get_target(TARGET(1), btrgb::TargetType::GENERAL_TARGET);
        target2 = images->get_target(TARGET(2), btrgb::TargetType::GENERAL_TARGET);
        ColorTarget targets[] = { target1, target2 };
        int channel_count = art1->channels();
        int target_count = std::size(targets);

        // Init Matracies used in calibration
        this->color_patch_avgs = btrgb::calibration::build_target_avg_matrix(targets, target_count, channel_count);
        this->build_input_matrix();
        comms->send_progress(0.1, this->get_name());
        this->deltaE_values = cv::Mat_<double>(target1.get_row_count(), target1.get_col_count(), CV_32FC1);

        // Fined M and Offsets to minimize deltaE
        std::cout << "Optimizing to minimize deltaE" << std::endl;
        this->find_optimization();
        comms->send_progress(0.6, this->get_name());
    }
    else {
        try {
            //If targets aren't found, this is a batch request and the information must be retrieved
            std::string directory = images->get_output_dir();
            std::cout << "Output directory: " << directory << std::endl;


            // Normalize directory separators to be platform independent
            std::filesystem::path normalizedDir = std::filesystem::path(directory).make_preferred();

            // Move up one directory level to get to the base directory (BeyondRGB_2024)
            std::filesystem::path baseDirectory = normalizedDir.parent_path().parent_path();

            std::cout << "Base directory: " << baseDirectory << std::endl;

            std::string prefix = "BeyondRGB_M_color_";
            std::string path;

            // Iterate over the base directory and its subdirectories
            for (const auto& entry : std::filesystem::recursive_directory_iterator(baseDirectory)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    std::cout << "Checking file: " << filename << std::endl;
                    if (filename.find(prefix) != std::string::npos) {
                        path = entry.path().string();
                        break; // Found the file, exit the loop
                    }
                }
            }

            if (path.empty()) {
                throw std::runtime_error("No file found with prefix: " + prefix);
            }

            std::ifstream input_file(path);
            if (!input_file.is_open()) {
                throw std::runtime_error("Unable to open file: " + path);
            }
            this->build_input_matrix();
            std::stringstream buffer;
            buffer << input_file.rdbuf();
            std::string file_content = buffer.str();
            if (!loadMatricesFromText(file_content)) {
                // Handle error: pre-calculated data could not be loaded
                throw std::runtime_error("Precalculated M and offset matrices could not be loaded.");
            }
            this->deltaE_values = cv::Mat_<double>(this->ref_data->get_row_count(), this->ref_data->get_col_count(), CV_32FC1);
            recalculateDeltaE(images);
        }
        catch (const std::exception& e) {
            // Handle any exceptions thrown by readFileIntoString or loadMatricesFromText
            std::cerr << "Error: " << e.what() << std::endl;
            // Depending on the severity, either handle the error and continue or re-throw the exception
            throw;
        }

    }

    // Use M and Offsets to convert the 6 channel image to a 3 channel ColorManaged image, for both art and target
    std::cout << "Converting 6 channels to ColorManaged RGB image." << std::endl;
    try {
        this->update_image(images);
        if (targetsFound or images->get_batch() == false) {
            this->update_target(images,targetsFound);
        }
    }
    catch(const std::exception& e) {
       throw ImgProcessingComponent::error(e.what(), this->get_name());
    }
    comms->send_progress(0.9, this->get_name());

    // Save resulting Matacies for latter use
    this->output_report_data(images);
    comms->send_progress(1, this->get_name());

    // Dont remove art1 and art2 from the ArtObject yet as they are still needed for spectral calibration

}

/**
 * @brief Sets up and runs the MinProblemSolver to optimize M and offsets for min deltaE
 *
 */
void ColorManagedCalibrator::find_optimization() {
    // OpenCV MinProblemSolver requires the function for optimization
    // To be held in a Ptr<cv::MinProblemSolver::Function> class
    // DeltaEFunction is a class that inherits cv::MinProblemSolver::Function
    // and defines double calc(const double* x)const;
    //      Where
    //          x is the InputArray
    //          and the return value of calc() is the average DeltaE
    // The MinProblemSolver will provide new values for the InputArray
    // and try to minimize deltaE

    // Init DeltaEFunction
    // All params are references owned by ColorManagedCalibrator
    // This allows for access to all results (M, offset, deltaE_values) once optimization is complete
    cv::Ptr<cv::MinProblemSolver::Function> ptr_F(new DeltaEFunction(
        &this->optimization_input,  // InputArray, contans values for both M and offset
        &this->color_patch_avgs,    // Average values for all color patches
        &this->offest,              // A Croping of InputArray whos view is just the offset values
        &this->M,                   // A croping of InputArray whose view is a 2d(3x6) matrix of M values
        this->ref_data,             //
        &this->deltaE_values
    ));

    // Init MinProblemSolver
    cv::Ptr<cv::DownhillSolver> min_solver = cv::DownhillSolver::create();
    min_solver->setFunction(ptr_F);
    cv::Mat step = (cv::Mat_<double>(1,24) <<
                        0.75,0.75,0.75,0.75,0.75,0.75,  // M
                        0.75,0.75,0.75,0.75,0.75,0.75,  // M
                        0.75,0.75,0.75,0.75,0.75,0.75,  // M
                        0.01,0.01,0.01,0.01,0.01,0.01   // Offsetr
                    );
    min_solver->setInitStep(step);
    min_solver->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 50000, 1e-10));

    // Optimize M and offset for minimized deltaE
    this->resulting_avg_deltaE = min_solver->minimize(this->optimization_input);

    cv::Ptr<DeltaEFunction> def = ptr_F.staticCast<DeltaEFunction>();
    this->solver_iteration_count = def->get_itteration_count();
}

/***
 * Convert the sixe channels in art1 and art2 into a ColorManaged RGB image
 * using the optimized M and offsets
 */
void ColorManagedCalibrator::update_image(btrgb::ArtObject* images) {
    std::cout << "Updating Image" << std::endl;
    btrgb::Image* art1 = images->getImage("art1");
    btrgb::Image* art2 = images->getImage("art2");
    btrgb::Image* art[2] = { art1, art2 };
    int height = art1->height();
    int width = art1->width();


    // Initialize 6xN Matrix to represen our 6 channal image
    // Each row represents a single channel and N is the number total pixles for each channel
    cv::Mat camra_sigs = btrgb::calibration::build_camra_signals_matrix(art, 2, 6, &this->offest);

    /**
    *   M is a 2d Matrix in the form
    *       m_1_1, m_1_2, ..., m_1_6
    *       m_2_1, m_2_2, ..., m_2_6
    *       m_3_1, m_3_2, ..., m_3_6
    *
    *   camra_sigs is a 2d Matrix in the form
    *       px1_ch1, px2_ch1, ..., pxN_ch1
    *       px1_ch2, px2_ch2, ..., pxN_ch2
    *       px1_ch3, px2_ch3, ..., pxN_ch3
    *       px1_ch4, px2_ch4, ..., pxN_ch4
    *       px1_ch5, px2_ch5, ..., pxN_ch5
    *       px1_ch6, px2_ch6, ..., pxN_ch6
    *
    *   cm_XYZ is a 2d Matrix resulting in M * six_chan in the form
    *       X1, X2, ..., XN
    *       Y1, Y2, ..., YN
    *       Z1, Z2, ..., ZN
    *
    */
    // Convert camra_sigs ColorManaged XYZ values
    cv::Mat cm_XYZ = this->M * camra_sigs;
    camra_sigs.release(); // No longer needed

    /* Convert result matrix to a standard, three-channel bitmap format. */
    cv::Mat result_im = cm_XYZ.t();
    result_im = result_im.reshape(3, height);
    result_im.convertTo(result_im, CV_32F);

    /* Convert from XYZ to target color space and clip. */
    btrgb::ColorProfiles::convert_to_color(result_im, this->color_space);

    /* Apply nonlinearity of the target color space. */
    btrgb::ColorProfiles::apply_gamma(result_im, this->color_space);

 
    std::string name = CM_IMAGE_KEY;
    /* Wrap in Image object for storing in the ArtObject. */
    btrgb::Image* cm_im = new btrgb::Image(name);
    cm_im->initImage(result_im);
    cm_im->setColorProfile(this->color_space);
    cm_im->setExifTags(art1->getExifTags());
    cm_im->setConversionMatrix(BTRGB_M_OPT, this->M);
    cm_im->setConversionMatrix(BTRGB_OFFSET_OPT, this->offest);

    /* Store in ArtObject and output. */
    images->setImage(name, cm_im);

    // Store img size in GeneralInfo
    CalibrationResults* results_obj = images->get_results_obj(btrgb::ResultType::GENERAL);
    results_obj->store_int(GI_IMG_ROWS, cm_im->getMat().rows);
    results_obj->store_int(GI_IMG_COLS, cm_im->getMat().cols);

}

void ColorManagedCalibrator::update_target(btrgb::ArtObject* images,bool targetsFound) {
    std::cout << "Updating Target" << std::endl;
    btrgb::Image* target1;
    btrgb::Image* target2;
    if (targetsFound) {
        target1 = images->getImage("target1");
        target2 = images->getImage("target2");
    }
    else {
        target1 = images->getImage("art1");
        target2 = images->getImage("art2");
    }
    btrgb::Image* target[2] = { target1, target2 };
    int height = target1->height();
    int width = target2->width();


    // Initialize 6xN Matrix to represen our 6 channal image
    // Each row represents a single channel and N is the number total pixles for each channel
    cv::Mat camra_sigs = btrgb::calibration::build_camra_signals_matrix(target, 2, 6, &this->offest);

    /**
    *   M is a 2d Matrix in the form
    *       m_1_1, m_1_2, ..., m_1_6
    *       m_2_1, m_2_2, ..., m_2_6
    *       m_3_1, m_3_2, ..., m_3_6
    *
    *   camra_sigs is a 2d Matrix in the form
    *       px1_ch1, px2_ch1, ..., pxN_ch1
    *       px1_ch2, px2_ch2, ..., pxN_ch2
    *       px1_ch3, px2_ch3, ..., pxN_ch3
    *       px1_ch4, px2_ch4, ..., pxN_ch4
    *       px1_ch5, px2_ch5, ..., pxN_ch5
    *       px1_ch6, px2_ch6, ..., pxN_ch6
    *
    *   cm_XYZ is a 2d Matrix resulting in M * six_chan in the form
    *       X1, X2, ..., XN
    *       Y1, Y2, ..., YN
    *       Z1, Z2, ..., ZN
    *
    */
    // Convert camra_sigs ColorManaged XYZ values
    cv::Mat cm_XYZ = this->M * camra_sigs;
    camra_sigs.release(); // No longer needed

    /* Convert result matrix to a standard, three-channel bitmap format. */
    cv::Mat result_im = cm_XYZ.t();
    result_im = result_im.reshape(3, height);
    result_im.convertTo(result_im, CV_32F);

    /* Convert from XYZ to target color space and clip. */
    btrgb::ColorProfiles::convert_to_color(result_im, this->color_space);

    /* Apply nonlinearity of the target color space. */
    btrgb::ColorProfiles::apply_gamma(result_im, this->color_space);


    std::string name = "ColorManagedTarget";
    /* Wrap in Image object for storing in the ArtObject. */
    btrgb::Image* cm_im = new btrgb::Image(name);
    cm_im->initImage(result_im);
    cm_im->setColorProfile(this->color_space);
    cm_im->setExifTags(target1->getExifTags());
    cm_im->setConversionMatrix(BTRGB_M_OPT, this->M);
    cm_im->setConversionMatrix(BTRGB_OFFSET_OPT, this->offest);

    /* Store in ArtObject and output. */
    images->setImage(name, cm_im);
}

void ColorManagedCalibrator::output_report_data(btrgb::ArtObject* images){
    // Compute Calibrated XYZ values

    cv::Mat offset_avg = btrgb::calibration::apply_offsets(this->color_patch_avgs, this->offest);
    cv::Mat cm_xyz = this->M * offset_avg;
    // C
    cv::Mat L_camera;
    cv::Mat a_camera;
    cv::Mat b_camera;
    cv::Mat L_ref;
    cv::Mat a_ref;
    cv::Mat b_ref;
    btrgb::calibration::fill_Lab_values(&L_camera, &a_camera, &b_camera,
                          &L_ref,    &a_ref,    &b_ref,
                          cm_xyz, this->ref_data);
    // Fetch Results Object to store results in
    CalibrationResults *results_obj = images->get_results_obj(btrgb::ResultType::CALIBRATION);
    
    // DeltaE Mean
    results_obj->store_double(CM_DELTA_E_AVG, this->resulting_avg_deltaE);
    // XYZ transformation matrix
    results_obj->store_matrix(CM_M, this->M);
    // Offsets
    results_obj->store_matrix(CM_OFFSETS, this->offest);
    // DeltaE for all Patches
    results_obj->store_matrix(CM_DLETA_E_VALUES, this->deltaE_values);
    // Camera Signals
    results_obj->store_matrix(CM_CAMERA_SIGS, this->color_patch_avgs);
    // CM XYZ
    results_obj->store_matrix(CM_XYZ, cm_xyz);
    // L_camera a_camera b_camera
    results_obj->store_matrix(L_CAMERA, L_camera);
    results_obj->store_matrix(a_CAMERA, a_camera);
    results_obj->store_matrix(b_CAMERA, b_camera);
    // L_ref a_ref b_ref
    results_obj->store_matrix(L_REF, L_ref);
    results_obj->store_matrix(a_REF, a_ref);
    results_obj->store_matrix(b_REF, b_ref);  
}

void ColorManagedCalibrator::build_input_matrix() {
    int row_count = 4;
    int col_count = 6;
    int item_count = row_count * col_count;

    /** OpenCV MinProblemSolver expects a 1d Matrix as an InputArray
    * this->optimazation_input will represetn that InputArray
    * We need 2 seperate Maticies from the InputArray M, and Offset
    * M is a 3 by 6 transformation matrix
    * Offset is a 1 by 6 Matrix that holds offset values
    * The values are as follows
    *
    *  M,1.25,0.25,0.25,0.1,0.1,0.1
    *  M,0.25,1.15,-0.1,0.1,0.1,0.1
    *  M,-0.25,-0.25,1.5,0.1,0.1,0.1
    *  offset,0.1,0.01,0.01,0.01,0.01,0.01
    *
    * All values will be stored in the InputArray and Matrix Headers will be created for M, and Offset
    * Thes headers will point to the values in the InputArray and will simply represent that data in a different format
    * An change in the this->optimazation_input will be represented in M and Offset
    */

    /** This is the original M matrix given for optimization. */
    // this->optimization_input = (cv::Mat_<double>(1,item_count)<<
    //                         /*M*/       1.25,0.25,0.25,0.1,0.1,0.1,
    //                         /*M*/       0.25,1.15,-0.1,0.1,0.1,0.1,
    //                         /*M*/       -0.25,-0.25,1.5,0.1,0.1,0.1,
    //                         /*Offset*/  0.01,0.01,0.01,0.01,0.01,0.01
    //                             );

    /** We get better results when we use this one instead of the original */
    this->optimization_input = (cv::Mat_<double>(1,item_count)<<
                            /*M*/       0.1, 0.1, 0.25, 0.5, 0.1, 0.1,
                            /*M*/       0.1, 0.1, 0.25, 0.1, 1.0, 0.1,
                            /*M*/       0.1, 0.1, 0.25, 0.1, 0.1, 0.5,
                            /*Offset*/  0.01,0.01,0.01,0.01,0.01,0.01
                                );


    // Create Matrix Header to Represents the 1d InputArray as a 2d Matrix for easy extraction of M and offset
    cv::Mat opt_as_2d = cv::Mat(this->optimization_input).reshape(0, row_count);
    // Create Matrix Header to represent the 2d Matix M that points to the values that are in the InputArray
    this->M = opt_as_2d(cv::Rect(0, 0, col_count, row_count - 1));
    // Create Matrix Header to represent the 1d Matrix offset that points to the values that are in the InputArray
    this->offest = opt_as_2d(cv::Rect(0, row_count - 1, col_count, 1));
    opt_as_2d.release();

}


////////////////////////////////////////////////////////////////////////////////
//                                DeltaE Function                             //
////////////////////////////////////////////////////////////////////////////////

int DeltaEFunction::itteration_count = 0;
DeltaEFunction::DeltaEFunction(cv::Mat* opt_in, cv::Mat* cp_avgs, cv::Mat* offeset, cv::Mat* M, RefData* ref_data, cv::Mat* delE_values){
    // NOTE: opt_in, M, offset, delE_values are all references
    // When the values of those matracies are updated here they are updated in ColorManagedCalibrator
    // This means that once optimization is complete ColorManagedCalibrator already has the resulting values
    // Se doc strings in ColorManagedCalibrator for details on each of these matracies
    this->opt_in = opt_in;
    this->color_patch_avgs = cp_avgs;
    this->ref_data = ref_data;
    this->M = M;
    this->offeset = offeset;
    this->delE_values = delE_values;
}

int DeltaEFunction::getDims()const{
    // Dimentions of the input array
    // M is 3x6 and offset is 1x6
    // InputArray is 1d so 6 colums * 4 rows
    int dim = 6*4;
    std::cout << "Dim: " << dim << std::endl;
    return dim;
}

bool ColorManagedCalibrator::loadMatricesFromText(const std::string& file_content) {
    std::istringstream iss(file_content);
    std::string line;
    std::vector<double> m_values;
    std::vector<double> offset_values;

    // Skip the header line for M_color matrix
    std::getline(iss, line); // This reads and discards the "M_color" header

    // Read the M_color matrix
    int read_lines = 0;
    while (std::getline(iss, line) && read_lines < 3) {
        std::istringstream line_stream(line);
        std::string value;
        while (std::getline(line_stream, value, ',')) {
            // Trim the value in place
            value.erase(0, value.find_first_not_of(" \n\r\t\f\v")); // Left trim
            value.erase(value.find_last_not_of(" \n\r\t\f\v") + 1); // Right trim
            if (!value.empty()) {
                try {
                    m_values.push_back(std::stod(value));
                }
                catch (const std::invalid_argument& ia) {
                    std::cout << "Invalid argument when converting value '" << value << "': " << ia.what() << std::endl;
                    return false;
                }
            }
        }
        ++read_lines;
    }

    // Skip the header line for M_color_offset matrix
    if (!std::getline(iss, line) || line.find("M_color_offset") == std::string::npos) {
        return false;
    }

    // Read the M_color_offset matrix
    if (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        std::string value;
        while (std::getline(line_stream, value, ',')) {
            // Trim the value in place
            value.erase(0, value.find_first_not_of(" \n\r\t\f\v")); // Left trim
            value.erase(value.find_last_not_of(" \n\r\t\f\v") + 1); // Right trim
            if (!value.empty()) {
                try {
                    offset_values.push_back(std::stod(value));
                }
                catch (const std::invalid_argument& ia) {
                    std::cout << "Invalid argument when converting value '" << value << "': " << ia.what() << std::endl;
                    return false;
                }
            }
        }
    }
    else {
        std::cout << "Failed to read M_color_offset values." << std::endl; // Debug
        return false;
    }

    // Check if we have the right amount of values for each matrix
    size_t expected_m_size = 3 * 6; // M is expected to be 3x6
    size_t expected_offset_size = 6; // offset is expected to be 1x6

    if (m_values.size() != expected_m_size || offset_values.size() != expected_offset_size) {
        std::cout << "Error: The sizes of the matrices are not as expected." << std::endl;
        std::cout << "Expected M size: " << expected_m_size << ", Actual M size: " << m_values.size() << std::endl;
        std::cout << "Expected Offset size: " << expected_offset_size << ", Actual Offset size: " << offset_values.size() << std::endl;
        return false; // Invalid sizes
    }

    // Create cv::Mat objects from the vectors
    this->M = cv::Mat(3, 6, CV_64F); // 3x6 matrix
    memcpy(this->M.data, m_values.data(), m_values.size() * sizeof(double));

    this->offest = cv::Mat(1, 6, CV_64F); // 1x6 matrix
    memcpy(this->offest.data, offset_values.data(), offset_values.size() * sizeof(double));


    if (!std::getline(iss, line)) {
        std::cout << "Failed to read line after M_color_offset values." << std::endl;
        return false;
    }


    if (!std::getline(iss, line) || line.find("color_patch_avgs") == std::string::npos) {
        std::cout << "Failed to read color_patch_avgs header." << std::endl;
        return false;
    }
    std::cout << "Skipped line: " << line << std::endl;

    // Read the color_patch_avgs
    std::vector<std::vector<double>> color_patch_avgs_rows;
    int row_count = 0;
    while (std::getline(iss, line) && row_count < 6) { // Assuming there are 6 rows
        std::cout << "Reading color_patch_avgs line: " << line << std::endl;
        std::istringstream line_stream(line);
        std::string value;
        std::vector<double> row_values;
        while (std::getline(line_stream, value, ',')) {
            // Trim and convert value as before
            // ... [value trimming and conversion code] ...
            row_values.push_back(std::stod(value));
        }
        color_patch_avgs_rows.push_back(row_values);
        ++row_count;
    }

    // Check if the rows have consistent number of columns
    size_t num_columns = color_patch_avgs_rows.empty() ? 0 : color_patch_avgs_rows[0].size();
    for (const auto& row : color_patch_avgs_rows) {
        if (row.size() != num_columns) {
            std::cout << "Inconsistent number of columns in color_patch_avgs rows." << std::endl;
            return false;
        }
    }

    // Create cv::Mat object for color_patch_avgs
    this->color_patch_avgs = cv::Mat(row_count, num_columns, CV_64F);
    for (int i = 0; i < row_count; ++i) {
        memcpy(this->color_patch_avgs.ptr<double>(i), color_patch_avgs_rows[i].data(), num_columns * sizeof(double));
    }


    return true;
}



double DeltaEFunction::calc(const double* x)const{
    this->itteration_count++;

    // Update what is in opt_in whit whats passed in for the InputArray
    for(int i = 0; i < this->opt_in->cols; i++){
        double val = x[i];
        this->opt_in->at<double>(0,i) = val;
    }
    // For Debuging, display M and Offset to ensure that the updates are represented
    // ColorManagedCalibrator m;
    // m.display_matrix(this->M, "M");
    // m.display_matrix(this->offeset, "Offset");

    /**
    * Befor we can comput DeltaE we need to comput the x,y,z values fro each color patch
    *    camera_xyz = M * (color_patch_avg - offset)
    * => camera_xyz = M * offset_avg
    * Where
    *   camera_xyz is a 2d Matrix in the form
    *       x_patch_1, x_patch_2, ..., x_patch_k
    *       y_patch_1, y_patch_2, ..., y_patch_k
    *       z_patch_1, z_patch_2, ..., z_patch_k
    *
    *   M is a 2d Matrix in the form
    *       m_1_1, m_1_2, ..., m_1_6
    *       m_2_1, m_2_2, ..., m_2_6
    *       m_3_1, m_3_2, ..., m_3_6
    *
    *   color_patch_avg is a 2d Matrix in the form
    *   (cp_avg is the average pixel value from the color target in the actual image)
    *       cp_avg_chan1_patch_1, cp_avg_chan1_patch_2, ..., cp_avg_chan1_patch_k
    *       cp_avg_chan2_patch_1, cp_avg_chan2_patch_2, ..., cp_avg_chan2_patch_k
    *       ...                 , ...                 , ..., ...
    *       cp_avg_chan6_patch_1, cp_avg_chan6_patch_2, ..., cp_avg_chan6_patch_k
    *
    *   Offset is a  1d Matrix in the form
    *       offset_1, offset_2, ..., offset_6
    *
    *   offset_avg is a 2d Matrix computed from subtracting offset from color_patch_avg
    *       cp_avg_chan1_patch_1 - offet_1, cp_avg_chan1_patch_2 - offet_1, ..., cp_avg_chan1_patch_k - offet_1
    *       cp_avg_chan2_patch_1 - offet_2, cp_avg_chan2_patch_2 - offet_2, ..., cp_avg_chan2_patch_k - offet_2
    *       ...                  - ...    , ...                 - ...     , ..., ...                  - ...
    *       cp_avg_chan6_patch_1 - offet_6, cp_avg_chan6_patch_2 - offet_6, ..., cp_avg_chan6_patch_k - offet_6
    */

   // Create offset_avg
   cv::Mat offset_avg = btrgb::calibration::apply_offsets(*this->color_patch_avgs, *this->offeset);

    // Compute camera_xyz
    cv::Mat_<double> xyz = *this->M * offset_avg;

    // // Establish vars for DeltaE calculation
    int row_count = this->ref_data->get_row_count();
    int col_count = this->ref_data->get_col_count();

    // Compute DeltaE values/sum
    double deltaE_sum = btrgb::calibration::compute_deltaE_sum(this->ref_data, xyz, this->delE_values);
    
    // Calculate the Average DeltaE
    int patch_count = row_count * col_count;
    double deltaE_avg = deltaE_sum / patch_count;
    return deltaE_avg;
}

//Used for calculating deltaE_sum and resulting_avg_deltaE in batch requests
void ColorManagedCalibrator::recalculateDeltaE(btrgb::ArtObject* images) {
    // Recalculate offset averages
    cv::Mat offset_avg = btrgb::calibration::apply_offsets(this->color_patch_avgs, this->offest);

    // Recalculate camera_xyz using current M and offset_avg
    cv::Mat_<double> xyz = this->M * offset_avg;

    // Recalculate deltaE values
    double deltaE_sum = btrgb::calibration::compute_deltaE_sum(this->ref_data, xyz, &this->deltaE_values);

    // Recalculate and update resulting_avg_deltaE
    int patch_count = this->deltaE_values.total(); // total number of elements in the deltaE matrix
    this->resulting_avg_deltaE = deltaE_sum / patch_count;
}
