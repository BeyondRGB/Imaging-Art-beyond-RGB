#ifndef SPECTRALCALIBRATOR_H
#define SPECTRALCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>

#include <reference_data/ref_data.hpp>

#include <image_processing/LeafComponent.h>

/**
 * @brief Runs the SpectralCalibration
 * when done outputs results to a Results object held by the given ArtObj
 * 
 */
class SpectralCalibrator : public LeafComponent{
public:
    SpectralCalibrator() : LeafComponent("Spectral Calibrating"){}
    /**
     * @brief Runs the calibration
     * 
     * @param func TODO this should be changed to the ComsObj once merged with the communication update
     * @param images contais Images to process
     */
    void execute(CommunicationObj *comms, btrgb::ArtObject* images) override;
    void my_callback(std::string str);

private:
    RefData* ref_data;
    cv::Mat color_patch_avgs;
    cv::Mat input_array; // 1d representation of M_refl
    cv::Mat M_refl; // 2d Croping of input_array
    cv::Mat R_camera; // Container for holding the R_camera values that this process creates

    /**
     * @brief Initialize the starting input_array and M_refl
     * The MinProblemSolver needs the InputArray to be a 1d Matrix but we need to optimize M_refl
     * This function creates M_refl and fills with initial values
     * It then creates a 1d representation (input_array) pointing to the values held in M_refl
     * If values in either are modified they are modified in both 
     * 
     *  M_refl is a 2d Matrix in the form
     *      m_1_1,  m_1_2,  ..., m_1_6
     *      m_2_1,  m_2_2,  ..., m_2_6
     *      ...  ,  ...  ,  ..., ...
     *      m_36_1, m_36_2, ..., m_36_6
     * 
     *  input_array is a 1d Matrix in the form
     *      m_1_1, m_1_2, ..., m_1_6, ..., m_36_6
     *      
     * 
     * @param R_ref a 2d matrix representation of RefData
     */
    void init_M_refl(cv::Mat R_ref);

    /**
     * @brief Initialize all values in given step Matrix to stp_values
     * 
     * @param stp_value the value to initialize to
     * @param step the matrix to set values in
     */
    void init_step(double stp_value, cv::Mat &step);

    /**
     * @brief Store the results from the calibration
     * 
     */
    void store_results(btrgb::ArtObject *images);

    /**
     * @brief Construct and strore a 6 channel spectral Image
     * 
     * @param images 
     */
    void store_spectral_img(btrgb::ArtObject *images);

    bool loadMatricesFromText(const std::string& file_content);
};


////////////////////////////////////////////////////////////////
//                      WeightedErrorFunction                 //
////////////////////////////////////////////////////////////////

/**
 * @brief Helper class that defines the function used for optimizing R_refl to minimize Z
 * Z is based off of minimizing the difference between values calculated using R_ref and the ground trueth values of the RefData
 * 
 * An instance of this class gets passed to the MinProblemSolver which calls calc during optimization
 * 
 * NOTE: all member variables used in optimization are passed by reference on construction.
 * This means that the creator of this class already has the results for R_refl and R_camera
 * at the time that optimization complets
 * 
 */
class WeightedErrorFunction: public cv::MinProblemSolver::Function{

public:
    WeightedErrorFunction(cv::Mat *ref_data, cv::Mat *input_array, cv::Mat *M_refl, cv::Mat *cp_carmera_sigs, cv::Mat *R_camera);
    int getDims() const;

    /**
     * @brief The function that the MinProblemSolver calls during optimization
     * 
     * @param x input values that the solver will modify during optimization
     * @return double the Z value computed using current input vlaue. This is what is beeing minimized.
     */
    double calc(const double* x) const;

    /**
     * @brief Get the itteration count object
     * 
     * @return int 
     */
    int get_itteration_count(){return this->itteration_count; }

private:
    static int itteration_count;// Keeps track of how many itteration the optimization completes
    cv::Mat *ref_data;
    cv::Mat *input_array;
    cv::Mat *M_refl;
    cv::Mat *cp_carmera_sigs;
    cv::Mat *R_camera;

    /**
     * @brief Calculate the Root Mean Square Error (RMSE)
     *  For every value in RefData/R_camera(actual signals)
     *      sum += (difference between RefData dn R_camra) rased to the power of 2
     * RMSE  = squar root of the sum
     * 
     * @return double RMSE
     */
    double calc_e1()const;

    /**
     * @brief Calculate e2 used to calculate Z
     *  For every wavelength(row) in RefData/R_camera(actual signals)
     *      e2 += (difference between max RefData value and max R_camera value) rased to the power of 2
     * e2 = the sum for all wavelengths
     * 
     * @return double
     */
    double calc_e2()const;

    /**
     * @brief Calcualte e3 used to calculate Z
     *  for every wavelength(row) in RefData/R_camera(actual signals)
     *      e3 += (difference between min RefData value and min R_camera value) rased to the poer of 2
     * e3 = the sum for all wavelengths
     * 
     * @return double 
     */
    double calc_e3()const;

    /**
     * @brief Calculate the Z value that we are trying to minimize
     * 
     * @param e1 
     * @param e2 
     * @param e3 
     * @return double 
     */
    double calc_z(double e1, double e2, double e3)const;
};


#endif //SPECTRALCALIBRATOR_H
