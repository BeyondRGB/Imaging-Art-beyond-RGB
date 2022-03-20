#include "../header/SpectralCalibrator.h"

void SpectralCalibrator::execute(CommunicationObj *comms, btrgb::ArtObject* images) {
    comms->send_info("", "SpectralCalibration");
    
    btrgb::Image* art1;
    btrgb::Image* art2;
    ColorTarget target1;
    ColorTarget target2;

    // Extract images and RefData from art object
    try {
        art1 = images->getImage(ART(1));
        art2 = images->getImage(ART(2));
        this->ref_data = images->get_refrence_data();
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        comms->send_info("Error: Flatfielding called out of order. Missing at least 1 image assignment.", "SpectralCalibration");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        comms->send_info("Error: " + error, "SpectralCalibration");
        return;
    }

    // Init Color Targets
    target1 = images->get_target(ART(1));
    target2 = images->get_target(ART(2));
    ColorTarget targets[] = { target1, target2 };
    int channel_count = art1->channels();
    int target_count = std::size(targets);

    //Init ColorTarget Averages
    this->color_patch_avgs = btrgb::calibration::build_target_avg_matrix(targets, target_count, channel_count);
    
    // // Run test with various step values
    // float sp_value = 0.5;
    // std::cout << std::endl << "****************************************************************" << std::endl;
    // std::cout << std::endl << "****************************************************************" << std::endl;

    // std::string str = "\nTesting Step: " + std::to_string(sp_value);
    // //func(str);
    // std::cout << str << std::endl;
    
    // Convert RefData to matrix
    cv::Mat ref_data_matrix = this->ref_data->as_matrix();

    // btrgb::calibration::display_matrix(&ref_data_matrix, "RefDataMat");

    // Initialize M_relf starting values
    this->init_M_refl(ref_data_matrix);

    // Create Custom WeightedErrorFunction used to minimize Z
    cv::Ptr<cv::MinProblemSolver::Function> ptr_F(new WeightedErrorFunction(
            &ref_data_matrix,
            &this->input_array,
            &this->M_refl,
            &this->color_patch_avgs,
            &this->R_camera
        )
    );

    //Init MinProblemSolver
    cv::Ptr<cv::DownhillSolver> min_solver = cv::DownhillSolver::create();
    min_solver->setFunction(ptr_F);
    cv::Mat step;
    this->init_step(0.5, step);
    min_solver->setInitStep(step);
    min_solver->setTermCriteria(
        cv::TermCriteria(
            cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, // Term Type
                5000, // max itterations
                1e-10 // epsilon
        )
    );


    // btrgb::calibration::display_matrix(&this->M_refl, "Mrefl Init");
    
    std::cout << "Running Minimization." << std::endl;

    TimeTracker time_tracker;
    time_tracker.start_timeing();
    // Optimize M_refl to minimized Z
    double res = min_solver->minimize(this->input_array);
    time_tracker.end_timeing();


    // Dsiplay Results
    btrgb::calibration::display_matrix(&this->M_refl, "Mrefl After");
    this->R_camera = this->M_refl * this->color_patch_avgs;
    btrgb::calibration::display_matrix(&this->R_camera, "RCamera After");
    this->R_camera = btrgb::calibration::calc_R_camera(this->M_refl, this->color_patch_avgs);
    btrgb::calibration::display_matrix(&this->R_camera, "RCamera Clipped");
    time_tracker.elapsed_time_sec();
    time_tracker.elapsed_time_min();

    cv::Ptr<WeightedErrorFunction> def = ptr_F.staticCast<WeightedErrorFunction>();
    std::cout << "Itterations: " << def->get_itteration_count() << std::endl;
    std::cout << "Min z: " << res << std::endl;
    

    std::cout << "SpectralCalibration done" << std::endl;

    this->store_results();

}

void SpectralCalibrator::init_M_refl(cv::Mat R_ref){
    int row_count = this->color_patch_avgs.rows;
    int col_count = this->color_patch_avgs.cols;
    // Init psudoinvers of ColorPatch Averages
    cv::Mat psudoinvers = cv::Mat_<double>(col_count, row_count, CV_32FC1);
    cv::invert(this->color_patch_avgs, psudoinvers, cv::DECOMP_SVD);

    // Create M_refl
    this->M_refl = R_ref * psudoinvers;
    // Create 1d representation of M_refl, used as input to the MinProblemSolver
    this->input_array = cv::Mat(this->M_refl).reshape(0,1);

    // btrgb::calibration::display_matrix(&R_ref, "R_ref");
    // btrgb::calibration::display_matrix(&this->color_patch_avgs, "ColorPatch CameraSigs");
    // btrgb::calibration::display_matrix(&psudoinvers, "Psudoinvers");
    // btrgb::calibration::display_matrix(&M_refl, "M_refl");
    
}

void SpectralCalibrator::init_step(double stp_value, cv::Mat &step){
    int step_len = this->M_refl.rows * this->M_refl.cols;
    step = cv::Mat_<double>(1,step_len);
    for(int i = 0; i < step_len; i++){
        step.at<double>(0,i) = stp_value;
    }
}

void SpectralCalibrator::store_results(){
    // TODO store the results in the ArtObj once that is merged in
    std::cout << "================================\n" <<
                 "Results have not been stored yet\n" <<
                 "================================" << std::endl;
}


////////////////////////////////////////////////////////////////
//                      WeightedErrorFunction                 //
////////////////////////////////////////////////////////////////

int WeightedErrorFunction::itteration_count = 0;

WeightedErrorFunction::WeightedErrorFunction(cv::Mat *ref_data, cv::Mat *input_array, cv::Mat *M_refl, cv::Mat *cp_carmera_sigs, cv::Mat *R_camera){
    /**
     * NOTE: input_array, M_refl, R_camera are all references
     * When the values of those matracies are updated here they are updated in SpectalCalibrator
     * This menas that once optimization is complete SpectralCalibrator already has the resulting values
     * See doc strings in SpectralCalibrator for details on each of these matracies 
     */
    this->ref_data = ref_data;
    this->input_array = input_array;
    this->M_refl = M_refl;
    this->cp_carmera_sigs = cp_carmera_sigs;
    this->R_camera = R_camera;
}

int WeightedErrorFunction::getDims() const{
    int dim = this->M_refl->rows * this->M_refl->cols;
    // std::cout << "GetDims: " << dim << std::endl;
    return dim;
}

double WeightedErrorFunction::calc(const double *x) const{
    this->itteration_count++;
    // Copy input into M_refl. x represents the changes made to the input data (M_refl)
    // M_refl is a 2d representation of input data
    for (int row = 0; row < this->M_refl->rows; row++){
        for(int col = 0; col < this->M_refl->cols; col++){
            int i = col + row * this->M_refl->cols;
            double val = x[i];
            this->M_refl->at<double>(row,col) = val;
        }
    }
    
    /**
     * Before we can compute Z we need to compute the R_camera values
     *  R_camera  = M_refl * cp_camera_sigs
     * 
     * Where
     *  cp_camera_sigs is a 2d Matrix in the form
     *   (cp_camera_sigs is the average pixel value from the color target in the ColorManaged image)
     *       cp_avg_chan1_patch_1, cp_avg_chan1_patch_2, ..., cp_avg_chan1_patch_k
     *       cp_avg_chan2_patch_1, cp_avg_chan2_patch_2, ..., cp_avg_chan2_patch_k
     *       ...                 , ...                 , ..., ...
     *       cp_avg_chan6_patch_1, cp_avg_chan6_patch_2, ..., cp_avg_chan6_patch_k 
     *    
     *  M_refl is a 2d Matrix in the form
     *      m_1_1,  m_1_2,  ..., m_1_6
     *      m_2_1,  m_2_2,  ..., m_2_6
     *      ...  ,  ...  ,  ..., ...
     *      m_36_1, m_36_2, ..., m_36_6
     * 
     *  R_camera is a 2d Matrix in fthe form
     *      RLamda_1_1,  RLamda_1_2,  ..., RLamda_1_k
     *      RLamda_2_1,  RLamda_2_2,  ..., RLamda_2_k
     *      ...       ,  ...       ,  ..., ...
     *      RLamda_36_1, RLamda_36_2, ..., RLamda_36_k
     *       
     */

    // Calculate R_camera from ColorPatch camera signals and current M_refl
    *this->R_camera = (*this->M_refl) * (*this->cp_carmera_sigs);
    
    // Calculate e values
    double e1 = this->calc_e1();
    double e2 = this->calc_e2();
    double e3 = this->calc_e3();

    // Calculate Z
    double z = this->calc_z(e1,e2,e3);

    // std::cout << "Z: " << z << " e1: " << e1 << " e2: " << e2 << " e3: " << e3 << std::endl; 
    return z;
}

double WeightedErrorFunction::calc_e1()const{
    double sum = 0;
    for(int row = 0; row < this->ref_data->rows; row++){
        for(int col = 0; col < this->ref_data->cols; col++){
            double cp_ref = this->ref_data->at<double>(row, col);
            double cp_camera = this->R_camera->at<double>(row,col);
            double cp_diff = cp_ref - cp_camera;
            sum += pow(cp_diff, 2);
            // std::cout << "cp_ref: " << cp_ref << " cp_camera: " << cp_camera << " cp_diff: " << cp_diff << " sum: " << sum << std::endl;
            // btrgb::calibration::enter_to_continue();
        }
    }
    double sqroot = sqrt(sum);
    // std::cout << "e1 sum: " << sum << " sqroot: " << sqroot << "   ";
    return sqroot;
}

double WeightedErrorFunction::calc_e2()const{
    double e2 = 0;
    for(int row = 0; row < this->ref_data->rows; row++){
        double camera_max = btrgb::calibration::row_max(*this->R_camera, row);
        double ref_max = btrgb::calibration::row_max(*this->ref_data, row);
        e2 += pow((camera_max - ref_max), 2);
    }
    return e2;
}

double WeightedErrorFunction::calc_e3()const{
    double e3 = 0;
    for(int row = 0; row < this->ref_data->rows; row++){
        double camera_min = btrgb::calibration::row_min(*this->R_camera, row);
        double ref_min = btrgb::calibration::row_min(*this->ref_data, row);
        e3 += pow((camera_min - ref_min), 2);
    }
    return e3;
}

double WeightedErrorFunction::calc_z(double e1, double e2, double e3)const{
    double z = e1 + 10 * e2 + 50 * e3;
    return z;
}
