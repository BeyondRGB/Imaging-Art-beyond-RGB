#include "../header/SpectralCalibrator.h"

void SpectralCalibrator::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("SpectralCalibration");
    
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
        func("Error: Flatfielding called out of order. Missing at least 1 image assignment.");
        return;
    }
    catch (const std::logic_error& e) {
        std::string error(e.what());
        func("Error: " + error);
        return;
    }

    // Init Color Targets
    target1 = images->get_target(ART(1));
    target2 = images->get_target(ART(2));
    ColorTarget targets[] = { target1, target2 };
    int channel_count = art1->channels();
    int target_count = std::size(targets);

    //Init Matracies used in calibration
    this->color_patch_avgs = btrgb::calibration::build_target_avg_matrix(targets, target_count, channel_count);
    this->init_M_refl();

    cv::Mat ref_data_matrix = this->ref_data->as_matrix();
    cv::Ptr<cv::MinProblemSolver::Function> ptr_F(new WeightedErrorFunction(
            &ref_data_matrix,
            &this->input_array,
            &this->M_refl,
            &this->color_patch_avgs,
            &this->R_camera
        ));

    //Init MinProblemSolver
    cv::Ptr<cv::DownhillSolver> min_solver = cv::DownhillSolver::create();
    min_solver->setFunction(ptr_F);
    for(float sp_value = 0.25f; sp_value <= 0.75f; sp_value += 0.1f){
        std::cout << "\nTesting Step: " << sp_value << std::endl;
        cv::Mat step;
        this->init_step(sp_value, step);
        // btrgb::calibration::display_matrix(&step, "Step");

        min_solver->setInitStep(step);
        min_solver->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 50000, 1e-10));

        // btrgb::calibration::display_matrix(&this->M_refl, "Mrefl");


        this->R_camera = this->M_refl * this->color_patch_avgs;
        // Optimize M and offset for minimized deltaE
        TimeTracker time_tracker;
        time_tracker.start_timeing();
        double res = min_solver->minimize(this->input_array);
        time_tracker.end_timeing();
        // btrgb::calibration::display_matrix(&this->R_camera, "RCamera Final");
        std::cout << "Min z: " << res << std::endl;

        time_tracker.elapsed_time_sec();
        time_tracker.elapsed_time_min();
    }
        

}

void SpectralCalibrator::init_M_refl(){
    std::cout << "Initializing M_refl" << std::endl;
    int row_count = this->color_patch_avgs.rows;
    int col_count = this->color_patch_avgs.cols;
    cv::Mat psudoinvers = cv::Mat_<double>(col_count, row_count, CV_32FC1);
    
    std::cout << "Invert" << std::endl;
    cv::invert(this->color_patch_avgs, psudoinvers, cv::DECOMP_SVD);
    // btrgb::calibration::display_matrix(&this->color_patch_avgs, "Patch AVGS");
    // btrgb::calibration::display_matrix(&psudoinvers, "psudoInvers");
    std::cout << "R_ref" << std::endl;
    cv::Mat R_ref = this->ref_data->as_matrix();
    std::cout << "Mrefl" << std::endl;
    this->M_refl = R_ref * psudoinvers;
    // btrgb::calibration::display_matrix(&this->M_refl, "M Reflectance");
    std::cout << "M_refl rows: " << this->M_refl.rows << " M_refl cols: " << M_refl.cols << std::endl;
    this->input_array = cv::Mat(this->M_refl).reshape(0,1);
    // btrgb::calibration::display_matrix(&this->input_array, "InputArray");
    std::cout << "InputArray rows: " << this->input_array.rows << " InputArray cols: " << input_array.cols << std::endl;
    
}

void SpectralCalibrator::init_step(double stp_value, cv::Mat &step){
    int step_len = this->M_refl.rows * this->M_refl.cols;
    step = cv::Mat_<double>(1,step_len);
    for(int i = 0; i < step_len; i++){
        step.at<double>(0,i) = stp_value;
    }
}


////////////////////////////////////////////////////////////////
//                      WeightedErrorFunction                 //
////////////////////////////////////////////////////////////////

WeightedErrorFunction::WeightedErrorFunction(cv::Mat *ref_data, cv::Mat *input_array, cv::Mat *M_refl, cv::Mat *cp_carmera_sigs, cv::Mat *R_camera){
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
    for (int row = 0; row < this->M_refl->rows; row++){
        for(int col = 0; col < this->M_refl->cols; col++){
            int i = col + row * this->M_refl->cols;
            double val = x[i];
            this->M_refl->at<double>(row,col) = val;
        }
    }
    // std::cout << "R_camera" << std::endl;
    *this->R_camera = (*this->M_refl) * (*this->cp_carmera_sigs);
    // btrgb::calibration::display_matrix(this->R_camera, "R_camera");
    // std::cout << "e cals" << std::endl;
    double e1 = this->calc_e1();
    double e2 = this->calc_e2();
    double e3 = this->calc_e3();

    // std::cout << "Z" << std::endl;
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
        }
    }
    return sqrt(sum);
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