#include "../header/ColorManagedCalibrator.h"

ColorManagedCalibrator::~ColorManagedCalibrator() {
}

void ColorManagedCalibrator::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("", "Color Managed Calibration");
    comms->send_progress(0, "Color Managed Calibration");

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
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        comms->send_error("ColorManagedCalibrator called out of order. Missing at least 1 image assignment.", "Color Managed Calibration");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        comms->send_error(error, "Color Managed Calibration");
        return;
    }

    // Init Color Targets
    target1 = images->get_target(ART(1), btrgb::TargetType::GENERAL_TARGET);
    target2 = images->get_target(ART(2), btrgb::TargetType::GENERAL_TARGET);
    ColorTarget targets[] = { target1, target2 };
    int channel_count = art1->channels();
    int target_count = std::size(targets);

    // Init Matracies used in calibration
    this->color_patch_avgs = btrgb::calibration::build_target_avg_matrix(targets, target_count, channel_count);
    this->build_input_matrix();
    comms->send_progress(0.1, "Color Managed Calibration");
    this->deltaE_values = cv::Mat_<double>(target1.get_row_count(), target1.get_col_count(),CV_32FC1);

    // Fined M and Offsets to minimize deltaE
    std::cout << "Optimizing to minimize deltaE" << std::endl;
    this->find_optimization();
    comms->send_progress(0.6, "Color Managed Calibration");

    // Use M and Offsets to convert the 6 channel image to a 3 channel ColorManaged image
    std::cout << "Converting 6 channels to ColorManaged RGB image." << std::endl;
    try { this->update_image(images); }
    catch(btrgb::ArtObj_ImageAlreadyExists e) {
       comms->send_error("Image already exists, could not save result.", "Color Managed Calibration");
    } catch(btrgb::ArtObj_FailedToWriteImage e) {
       comms->send_error("Failed to write image.", "Color Managed Calibration");
    } comms->send_progress(0.9, "Color Managed Calibration");

    // Save resulting Matacies for latter use
    this->output_report_data(images);
    comms->send_progress(1, "Color Managed Calibration");

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
void ColorManagedCalibrator::update_image(btrgb::ArtObject* images){
    std::cout << "Updating Image" << std::endl;
    btrgb::Image* art1 = images->getImage("art1");
    btrgb::Image* art2 = images->getImage("art2");
    btrgb::Image* art[2] = {art1, art2};
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

    /* Store in ArtObject and output. */
    images->setImage(name, cm_im);
    
    // Store img size in GeneralInfo
    CalibrationResults *results_obj = images->get_results_obj(btrgb::ResultType::GENERAL);
    results_obj->store_int(GI_IMG_ROWS, cm_im->getMat().rows);
    results_obj->store_int(GI_IMG_COLS, cm_im->getMat().cols);

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
    this->fill_Lab_values(&L_camera, &a_camera, &b_camera,
                          &L_ref,    &a_ref,    &b_ref,
                          cm_xyz);
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


void ColorManagedCalibrator::fill_Lab_values(cv::Mat *L_camera, cv::Mat *a_camera, cv::Mat *b_camera,
                                             cv::Mat *L_ref,    cv::Mat *a_ref,    cv::Mat *b_ref,
                                              cv::Mat xyz){
    int row_count = this->ref_data->get_row_count();
    int col_count = this->ref_data->get_col_count();
    *L_camera = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *b_camera = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *a_camera = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *L_ref = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *a_ref = cv::Mat_<double>(row_count, col_count, CV_64FC1);
    *b_ref = cv::Mat_<double>(row_count, col_count, CV_64FC1);

    WhitePoints* wp = this->ref_data->get_white_pts();
    for(int row = 0; row < row_count; row++){
        for(int col = 0; col < col_count; col++){
            // Get/Store L*,a*,b* values from RefData
            L_ref->at<double>(row,col) = this->ref_data->get_L(row, col);
            a_ref->at<double>(row,col) = this->ref_data->get_a(row, col);
            b_ref->at<double>(row,col) = this->ref_data->get_b(row, col);

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

    // Establish vars for DeltaE calculation
    int row_count = this->ref_data->get_row_count();
    int col_count = this->ref_data->get_col_count();
    double ref_L;
    double ref_a;
    double ref_b;
    double L;
    double a;
    double b;

    // Calculate AVG delta E for all ColorPatches on target
    // delta E is the difference in color between the RefData and the actual image Target(xyz Mat)
    WhitePoints* wp = this->ref_data->get_white_pts();
    double deltaE_sum = 0;
    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < col_count; col++) {
            // Get L*,a*,b* values from RefData
            ref_L = this->ref_data->get_L(row, col);
            ref_a = this->ref_data->get_a(row, col);
            ref_b = this->ref_data->get_b(row, col);

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
            this->delE_values->at<double>(row,col) = delE;
            deltaE_sum += delE;
        }
    }

    // Calculate the Average DeltaE
    int patch_count = row_count * col_count;
    double deltaE_avg = deltaE_sum / patch_count;
    return deltaE_avg;
}
