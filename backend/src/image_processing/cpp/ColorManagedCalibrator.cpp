

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

    // Set to default for now.
    // This should be updated at some point to be settable by user
    this->color_space = ColorSpace::ProPhoto;

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

    // Init Matracies used in calibration
    this->build_target_avg_matrix(targets, target_count, channel_count);
    this->build_input_matrix();
    this->deltaE_values = cv::Mat_<double>(target1.get_row_count(), target1.get_col_count(),CV_32FC1);

    // Fined M and Offsets to minimize deltaE
    this->find_optimization();
    
    // Use M and Offsets to convert the 6 channel image to a 3 channel ColorManaged image
    this->update_image(images);

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
                        1.75,1.75,1.75,1.75,1.75,1.75,  // M
                        1.75,1.75,1.75,1.75,1.75,1.75,  // M
                        1.75,1.75,1.75,1.75,1.75,1.75,  // M
                        0.01,0.01,0.01,0.01,0.01,0.01   // Offsetr
                    );                                
    min_solver->setInitStep(step);
    min_solver->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 50000, 1e-10));
    
    // Optimize M and offset for minimized deltaE
    double res = min_solver->minimize(this->optimization_input);


    /** TODO Remove below when ready to merge */ 
    std::cout << "**********************\n\tResults\n**********************" << std::endl;
    this->display_matrix(&this->M, "M");
    //std::cout << "Resulting offset" << std::endl;
    this->display_matrix(&this->offest, "offset");
    this->display_matrix(&this->deltaE_values, "DelE Values");
    cv::Ptr<DeltaEFunction> def = ptr_F.staticCast<DeltaEFunction>();
    std::cout << "Resulting DeltaE: " << res << std::endl;
    std::cout << "Itteration Count: " << def->get_itteration_count() << std::endl;
    std::cout << "\n*********************************************************************************************************************" << std::endl;

}

/***
 * Convert the sixe channels in art1 and art2 into a ColorManaged RGB image 
 * using the optimized M and offsets
 */
void ColorManagedCalibrator::update_image(btrgb::ArtObject* images){
    std::cout << "Updating Image" << std::endl;
    btrgb::Image* art1 = art1 = images->getImage("art1");
    btrgb::Image* art2 = images->getImage("art2");
    btrgb::Image* art[2] = {art1, art2};
    int height = art1->height();
    int width = art1->width();
    
    // Initialize 6xN Matrix to represen our 6 channal image
    // Each row represents a single channel and N is the number total pixles for each channel
    cv::Mat six_chan = cv::Mat_<double>(6, height * width, CV_32FC1);
    int chan_count = 3; // Each image only has 3 channels
    for(int art_i = 0; art_i < std::size(art); art_i++){
        // The are image we are currently getting pixel values from
        btrgb::Image* art_c = art[art_i];
        for(int chan = 0; chan < chan_count; chan++){
            // The row in the six_chan matrix we are currently adding values to
            int mat_row = chan + art_i * chan_count;
            for(int row = 0; row < art_c->height(); row++){
                int offset_index = chan + art_i * 3;
                // The offset value subracted from each pixel
                double offset_value = this->offest.at<double>(offset_index);
                for(int col = 0; col < art_c->width(); col++){
                    // The pixel value we are going to set. 
                    // NOTE: this includes the subraction of the offset_value
                    double px_val = (double)art_c->getPixel(row, col, chan) - offset_value;
                    // The col in the six_chan matrix we ar currently adding values to
                    int mat_col = col + row * art_c->width();
                    // Set pixel
                    six_chan.at<double>(mat_row, mat_col) = px_val;
                }
            }
        }
    }

    /**
    *   M is a 2d Matrix in the form
    *       m_1,1, m_1,2, ..., m_1,6
    *       m_2,1, m_2,2, ..., m_2,6
    *       m_3,1, m_3,2, ..., m_3,6
    * 
    *   six_chan is a 2d Matrix in the form
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
    // Convert six channels ColorManaged XYZ values
    cv::Mat cm_XYZ = this->M * six_chan;
    six_chan.release(); // No longer needed
    
    // Convert ColorManaged XYZ values to ColorManaged RGB values
    cv::Mat rgb_convertion_matrix = this->rgb_convertions_matrix(this->color_space);
    cv::Mat cm_RGB = rgb_convertion_matrix * cm_XYZ;
    cm_XYZ.release(); // No longer neeeded

    std::cout << "Creating Image" << std::endl;

    // We hav now converted all vaues to RGB but they are still in a format that is unusable
    // Copy all values from cm_RGB into a usable format(cm_im)
    // cm_im is the actual Image that will contain our final output
    btrgb::Image* cm_im = new btrgb::Image("ColorManaged");
    cv::Mat color_managed_data(height, width, CV_32FC3);
    cm_im->initImage(color_managed_data);
    for(int chan = 0; chan < 3; chan++){
        for(int row = 0; row < height; row++){
            for(int col = 0; col < width; col++){
                int data_col = col + row * width;
                float px_value = (float)cm_RGB.at<double>(chan, data_col);
                // Apply gamma to correct brightness
                float gamma_corrected_value = this->apply_gamma(px_value, this->color_space);
                cm_im->setPixel(row, col, chan, gamma_corrected_value);
            }
        }
    }
    cm_RGB.release(); // No longer needed

    // Store New Image and write to TIFF
    std::string name = "ColorManaged";
    try{
        images->setImage(name, cm_im);
        images->outputImageAs(btrgb::output_type::TIFF, name);
    }catch(btrgb::ArtObj_ImageAlreadyExists e){
        std::cout << "Fail to set Img: " << e.what() << std::endl;
    }catch(btrgb::ArtObj_FailedToWriteImage e){
        std::cout << "Fail to write Img: " << e.what() << std::endl;
    }catch(std::exception e){
        std::cout << "Fail to write Img: " << e.what() << std::endl;
    }
}

// ColorTarget ColorManagedCalibrator::get_target(btrgb::ArtObject* images, btrgb::Image* im) {
//     // TODO refactor ArtObject to hold all ColorTargets
//     TargetData target_data;
//     // Extract nessisary data from ArtObject about color target and stor it in target_data
//     try {
//         target_data.top_loc = images->getTargetInfo("top");
//         target_data.bot_loc = images->getTargetInfo("bot");
//         target_data.left_loc = images->getTargetInfo("left");
//         target_data.right_loc = images->getTargetInfo("right");

//         target_data.row_count = images->getTargetSize("row");
//         target_data.col_count = images->getTargetSize("col");
//     }
//     catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
//         throw e;
//     }
//     catch (const std::logic_error& e) {
//         throw e;
//     }
//     // Create and return ColorTarget
//     ColorTarget target(im, target_data);
//     return target;
// }

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
                }
            }
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

    // this->optimization_input = (cv::Mat_<double>(1,item_count)<<
    //                         /*M*/       0.1, 0.1, 0.25, 0.5, 0.1, 0.1,
    //                         /*M*/       0.1, 0.1, 0.25, 0.1, 1.0, 0.1,
    //                         /*M*/       0.1, 0.1, 0.25, 0.1, 0.1, 0.5,
    //                         /*Offset*/  0.01,0.01,0.01,0.01,0.01,0.01
    //                             );
   
     
    // Create Matrix Header to Represents the 1d InputArray as a 2d Matrix for easy extraction of M and offset
    cv::Mat opt_as_2d = cv::Mat(this->optimization_input).reshape(0, row_count);
    // Create Matrix Header to represent the 2d Matix M that points to the values that are in the InputArray
    this->M = opt_as_2d(cv::Rect(0, 0, col_count, row_count - 1));
    // Create Matrix Header to represent the 1d Matrix offset that points to the values that are in the InputArray
    this->offest = opt_as_2d(cv::Rect(0, row_count - 1, col_count, 1));  
    opt_as_2d.release();
    
}

cv::Mat ColorManagedCalibrator::rgb_convertions_matrix(ColorSpace color_space){
    /**
     * @brief There are various xyz to rgb convertions matracies depending on the color space 
     * the converiton matracies defined below come from http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
     */
    cv::Mat convertions_matrix;
    switch(color_space){
        case ColorSpace::Adobe_RGB_1998:
            convertions_matrix = (cv::Mat_<double>(3,3) << 
                             1.9624274, -0.6105343, -0.3413404,
                            -0.9787684,  1.9161415,  0.0334540,
                             0.0286869, -0.1406752,  1.3487655
                    );
            break;

        case ColorSpace::Wide_Gamut_RGB:
            convertions_matrix = (cv::Mat_<double>(3,3) << 
                             1.4628067, -0.1840623, -0.2743606,
                            -0.5217933,  1.4472381,  0.0677227,
                             0.0349342, -0.0968930,  1.2884099
                    );
            break;

        case ColorSpace::sRGB:
            convertions_matrix = (cv::Mat_<double>(3,3) << 
                             3.1338561, -1.6168667, -0.4906146,
                            -0.9787684,  1.9161415,  0.0334540,
                             0.0719453, -0.2289914 , 1.4052427
                    );
            break;

        case ColorSpace::ProPhoto:
        default:
            convertions_matrix = (cv::Mat_<double>(3,3) << 
                             1.3459433, -0.2556075, -0.0511118,
                            -0.5445989,  1.5081673,  0.0205351,
                             0.0000000,  0.0000000,  1.2118128
                    );
            break;
    }
    return convertions_matrix;
}

float ColorManagedCalibrator::gamma(ColorSpace color_space){
    //TODO the gamma values included here are not what they should be and should
    // be updated once we know what they are.
    float gamma = 2.2;
    switch(color_space){
        case ColorSpace::Adobe_RGB_1998:
            break;

        case ColorSpace::Wide_Gamut_RGB:
            break;

        case ColorSpace::sRGB:
            break;

        case ColorSpace::ProPhoto:
        default:
            gamma = 1.8;
            break;
    }
    return gamma;
}

float ColorManagedCalibrator::apply_gamma(float px_value, ColorManagedCalibrator::ColorSpace color_space){
    // TODO this is not complete yet and is more complicated than what is currently implemented
    // Update this once we know what is involved
    float gamma = this->gamma(color_space);
    // Apply gamma to correct brightness
    float gamma_corrected_value = std::pow(px_value, gamma);
    return gamma_corrected_value;
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
            std::cout << std::endl;// << std::endl;
        }
    }
    else {
        std::cout << "Matrix not initialized" << std::endl;
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
    cv::Mat_<double> offset_avg(row_count, col_count, CV_64FC1);
    for (int row = 0; row < row_count; row++) {
        double offset = this->offeset->at<double>(row);
        for (int col = 0; col < col_count; col++) {
            double avg = this->color_patch_avgs->at<double>(row, col);
            offset_avg.at<double>(row, col) = avg - offset;
        }
    }
    
    // Compute camera_xyz
    cv::Mat_<double> xyz = *this->M * offset_avg;

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
            // Store value in matrix. This matrix will hold the actual deltaE values for each patch for the min avg found
            this->delE_values->at<double>(row,col) = delE;
            deltaE_sum += delE;
        }
    }
    
    // Calculate the Average DeltaE
    int patch_count = row_count * col_count;
    double deltaE_avg = deltaE_sum / patch_count;
    //std::cout << "DeltaE: " << deltaE_avg << std::endl;
    //enter_to_continue();
    return deltaE_avg;
}

