

#include "../header/ColorManagedCalibrator.h"



void enter_to_continue(){
    std::cout << "Enter To Continue.";
    char c;
    do{
        std::cin >> std::noskipws >> c;
        std::cout << "c: (" << c << ")" << std::endl;
    }while(c != '\n');
}

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
    this->display_matrix(&this->color_patch_avgs, "ColorPatch Avgs");
    
    this->deltaE_values = cv::Mat_<double>(target1.get_row_count(), target1.get_col_count(),CV_32FC1);

    this->build_input_matrix();

    this->display_matrix(&this->optimization_input, "InputArray");
    this->display_matrix(&this->M, "M");
    this->display_matrix(&this->offest, "Offset");

    this->find_optimization();
    

}

void ColorManagedCalibrator::find_optimization() {
    // std::cout << "Initial M" << std::endl;
    // this->display_matrix(&this->M, "M");
    // std::cout << "Creating Function" << std::endl;

    // OpenCV MinProblemSolver requires the function for optimization
    // To be held in a Ptr<cv::MinProblemSolver::Function> class
    // DeltaEFunction is a class that inherits cv::MinProblemSolver::Function
    // and defines double calc(const double* x)const;
    //      Where 
    //          x is the InputArray
    //          and the return value of calc() is the average DeltaE
    // The MinProblemSolver will provide new values for the InputArray
    // and try to minimize deltaE
    

    cv::Ptr<cv::MinProblemSolver::Function> ptr_F(new DeltaEFunction(
        &this->optimization_input, 
        &this->color_patch_avgs, 
        &this->offest, 
        &this->M, 
        this->ref_data, 
        &this->deltaE_values
    ));
    // ptr_F->calc(&this->optimization_input.data.db);
    
    std::cout << "Creating Solver" << std::endl;
    //cv::Ptr<cv::ConjGradSolver> min_solver = cv::ConjGradSolver::create();
    cv::Ptr<cv::DownhillSolver> min_solver = cv::DownhillSolver::create();
    std::cout << "Setting function" << std::endl;
    min_solver->setFunction(ptr_F);
    cv::Mat step = (cv::Mat_<double>(1,24)<<
                            /*M*/       1.75,1.75,1.75,1.75,1.75,1.75,
                            /*M*/       1.75,1.75,1.75,1.75,1.75,1.75,
                            /*M*/       1.75,1.75,1.75,1.75,1.75,1.75,
                            /*Offset*/  0.01,0.01,0.01,0.01,0.01,0.01);
                                
    min_solver->setInitStep(step);
    min_solver->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 50000, 1e-10));
    std::cout << "Minimizing" << std::endl;
    double res = min_solver->minimize(this->optimization_input);


    std::cout << "Resulting DeltaE: " << res << std::endl;
    std::cout << "Resulting M" << std::endl;
    this->display_matrix(&this->M, "M");
    std::cout << "Resulting offset" << std::endl;
    this->display_matrix(&this->offest, "offset");
    this->display_matrix(&this->deltaE_values, "DelE Values");
}

ColorTarget ColorManagedCalibrator::get_target(btrgb::ArtObject* images, btrgb::Image* im) {
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
    // Create and return ColorTarget
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

    this->color_patch_avgs = cv::Mat_<double>(mat_row_count, mat_col_count, CV_32FC1);
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
                    this->color_patch_avgs.at<double>(mat_row, mat_col) = (double)avg;
                    std::cout << avg << ",";
                }
            }
            std::cout << std::endl << std::endl;
        }
    }
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
    this->optimization_input = (cv::Mat_<double>(1,item_count)<<
                            /*M*/       1.25,0.25,0.25,0.1,0.1,0.1,
                            /*M*/       0.25,1.15,-0.1,0.1,0.1,0.1,
                            /*M*/       -0.25,-0.25,1.5,0.1,0.1,0.1,
                            /*Offset*/  0.01,0.01,0.01,0.01,0.01,0.01
                                );
     
    // Create Matrix Header to Represents the 1d InputArray as a 2d Matrix for easy extraction of M and offset
    cv::Mat opt_as_2d = cv::Mat(this->optimization_input).reshape(0, row_count);
    // Create Matrix Header to represent the 2d Matix M that points to the values that are in the InputArray
    this->M = opt_as_2d(cv::Rect(0, 0, col_count, row_count - 1));
    // Create Matrix Header to represent the 1d Matrix offset that points to the values that are in the InputArray
    this->offest = opt_as_2d(cv::Rect(0, row_count - 1, col_count, 1));

    //std::cout << "row_count: " << row_count << " col_count: " << col_count << std::endl;
    
    this->display_matrix(&this->optimization_input, "InputArray");
    this->display_matrix(&opt_as_2d, "InputArray 2d representation");
    this->display_matrix(&this->M, "M");
    this->display_matrix(&this->offest, "Offset");
}

void ColorManagedCalibrator::display_matrix(cv::Mat* matrix, std::string name) {
    std::cout << std::endl;
    std::cout << "What is in " << name << std::endl;
    if (nullptr != matrix) {
        for (int chan = 0; chan < matrix->rows; chan++) {
            for (int col = 0; col < matrix->cols; col++) {
                if (col != 0) {
                    std::cout << ", ";
                }
                double avg = matrix->at<double>(chan, col);
                std::cout << avg;
            }
            std::cout << std::endl << std::endl;
        }
    }
    else {
        std::cout << "Matrix not initialized" << std::endl;
    }
}

DeltaEFunction::DeltaEFunction(cv::Mat* opt_in, cv::Mat* cp_avgs, cv::Mat* offeset, cv::Mat* M, RefData* ref_data, cv::Mat* delE_values){
    // this->opt_in = opt_in;
    std::cout << "Initializing DeltaEFunction" << std::endl;
    this->opt_in = opt_in;
    this->color_patch_avgs = cp_avgs;
    this->ref_data = ref_data;
    // int row_count = this->color_patch_avgs->rows;//this->color_patch_avgs.rows;
    // int col_count = this->color_patch_avgs->cols;
    this->M = M;
    this->offeset = offeset;
    this->delE_values = delE_values;
}

int DeltaEFunction::getDims()const{
    int dim = 6*4;
    std::cout << "Dim: " << dim << std::endl;
    return dim;
}

double DeltaEFunction::calc(const double* x)const{
    // Update what is in opt_in whit whats passed in for the InputArray
    for(int i = 0; i < this->opt_in->cols; i++){
        double val = x[i];
        this->opt_in->at<double>(0,i) = val;
    }
    // For Debuging, display M and Offset to ensure that the updates are represented
     ColorManagedCalibrator m;
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
    *       m_1,1, m_1,2, ..., m_1,6
    *       m_2,1, m_2,2, ..., m_2,6
    *       m_3,1, m_3,2, ..., m_3,6
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
   int row_count = this->color_patch_avgs->rows;
    int col_count = this->color_patch_avgs->cols;
    //std::cout << "row_count: " << row_count << " col_count: " << col_count << std::endl;
    cv::Mat_<double> offset_avg(row_count, col_count, CV_64FC1);
    for (int row = 0; row < row_count; row++) {
        double offset = this->offeset->at<double>(row);
        for (int col = 0; col < col_count; col++) {
            //::cout << "(row,col): (" << row << "," << col << ")" << std::endl;
            double avg = this->color_patch_avgs->at<double>(row, col);
            offset_avg.at<double>(row, col) = avg - offset;
        }
    }
    
    // Compute camera_xyz
    cv::Mat_<double> xyz = *this->M * offset_avg;
    // m.display_matrix(&xyz, "XYZ");
    // for(int row = 0; row < xyz.rows; row++){
    //     for(int col = 0; col < xyz.cols; col++){
    //         double scaled_value = xyz.at<double>(row,col) * 100;
    //         xyz.at<double>(row,col) = scaled_value;
    //     }
    // }
    // m.display_matrix(&xyz, "Scaled XYZ");

    // Establish vars for DeltaE calculation
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
            cmsCIELab lab1(ref_L, ref_a, ref_b);
            cmsCIELab lab2(lab.L, lab.a, lab.b);
            double delE = cmsCIE2000DeltaE(&lab1, &lab2, 1, 1, 1);
            //std::cout << "Adding(" << col << "," << row<< " ): " << delE << std::endl;
            // Store value in matrix. This matrix will hold the actual deltaE values for each patch for the min avg found
            this->delE_values->at<double>(row,col) = delE;
            deltaE_sum += delE;
           // enter_to_continue();
            
        }
    }
    
    

    // Calculate the Average DeltaE
    int patch_count = row_count * col_count;
    double deltaE_avg = deltaE_sum / patch_count;
    
    // if(deltaE_avg < 1.35)
    //     m.display_matrix(this->delE_values, "delE_Values update");
    //std::cout << "DeltaE: " << deltaE_avg << std::endl;
    //enter_to_continue();
    return deltaE_avg;
}

void DeltaEFunction::crop_input(const double* x){
    // cv::Mat opt_as_2d = cv::Mat(x).reshape(0,4);
    // std::cout << "opt_as_2d" << std::endl;
    // this->M = opt_as_2d(cv::Rect(0, 0, 6, 4 - 1));
    // this->offeset = opt_as_2d(cv::Rect(0, 4 - 1, 6, 1));
}

// void DeltaEFunction::getGradient(const double* x, double* grad){
//     std::cout << "GetGradient" << std::endl;
// }