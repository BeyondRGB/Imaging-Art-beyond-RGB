#ifndef BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
#define BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H

#include <opencv2/core/optim.hpp>
#include <opencv2/opencv.hpp>

#include <image_processing/LeafComponent.h>
#include <image_util/ColorProfiles.hpp>
#include <reference_data/ref_data.hpp>
#include <server/communication_obj.hpp>

class ColorManagedCalibrator : public LeafComponent {
    /**
     * @brief Class to manage ColorManagedCalibration of images
     * It takes the 2 art images after they have bee through preprocessing
     * Optimizes some transfromation matracies then uses those matraceis to
     * convert the 6 channels from the art images to a color managed rgb image
     */

  public:
    ColorManagedCalibrator() : LeafComponent("Color Calibrating") {}
    ~ColorManagedCalibrator();
    void execute(CommunicationObj *comms, btrgb::ArtObject *images) override;

  private:
    cv::Mat optimization_input; // Contains M and offset values in a 1D matrix
    cv::Mat M;      // 2D Croping of optimazation_input if values are changed in
                    // either the other will be changed
    cv::Mat offest; // Croping of optimization_input if values are changed in
                    // either the other will be changed

    cv::Mat deltaE_values; // Container for holding the resulting deltaE values
                           // for each color patch
    cv::Mat color_patch_avgs;
    RefData *ref_data;

    double resulting_avg_deltaE;
    int solver_iteration_count;

    btrgb::ColorSpace color_space;

    double stp;
    int mid;

    /**
     * @brief Initialize the optimization InputArray(optimization_input), M, and
     * offset The MinProblemSolver needs the InputArray to be a 1d Matrix but we
     * need to optimiz both M and offset This function creates
     * optimization_input and fills it with initial starting values for both M
     * and offset It then creates a formatted cropping for M and offsets that
     * point to the values in the InputArray
     *
     *  optimization_input is a 1d Matrix in the form
     *      m_1,1, m_1,2, ..., m_1,6, ..., m_3,6, offset_1, offset_2, ...,
     * offset_6
     *
     *   M is a 2d Matrix in the form
     *       m_1_1, m_1_2, ..., m_1_6
     *       m_2_1, m_2_2, ..., m_2_6
     *       m_3_1, m_3_2, ..., m_3_6
     *
     *  Offset is a  1d Matrix in the form
     *      offset_1, offset_2, ..., offset_6
     *
     *
     */
    void build_input_matrix();

    /**
     * @brief Runs the MinProblemSolver to optimize M and offsets for a minimal
     * deltaE This requires that build_input_matrix has been called to set up
     * the InputArray, M, and offsets as well as build_target_avg_matrix, and
     * initialization of deltaE_values
     *
     * When Optimization is complete all results for M, offset, and
     * deltaE_values will already be held in those data structurs
     */
    void find_optimization();

    /**
     * @brief Uses the optimized M and offsets to convert the 6 channels
     * from art1 and art2 into a color managed RGB image
     *
     * @param images the art object containing art1 and art2
     */
    void update_image(btrgb::ArtObject *images);
    /**
     * @brief Uses the optimized M and offsets to convert the 6 channels
     * from target1 and target2 into a color managed RGB image
     *
     * @param images the art object containing target1 and target2
     */
    void update_target(btrgb::ArtObject *images, bool targetsFound);

    /**
     * @brief Saves optimized M and offset as well the final deltaE values
     *
     */
    void output_report_data(btrgb::ArtObject *images);

    bool loadMatricesFromText(const std::string &file_content);

    void recalculateDeltaE(btrgb::ArtObject *images);
};

/////////////////////////////////////////////////////////////////////////////////
//                                DeltaE Function                             //
////////////////////////////////////////////////////////////////////////////////

class DeltaEFunction : public cv::MinProblemSolver::Function {
    /**
     * @brief Helper class that defines the function used for optimizing M and
     * offset to minimize deltaE deltaE is the difference in color between what
     * we have(from cp_avgs) and what we are expecting(from RefData)
     *
     * An instance of this class gets passed to the MinProblemSolver which calls
     * calc during optimization
     *
     * NOTE: all member variables used in optimization are passed by referenc on
     * construction. This means that the creator of this class already has the
     * results for M, offset, and delE_values at the time that optimization
     * complets
     *
     */

  public:
    DeltaEFunction(cv::Mat *opt_in, cv::Mat *cp_avgs, cv::Mat *offeset,
                   cv::Mat *M, RefData *ref_data, cv::Mat *delE_values);
    int getDims() const;
    /**
     * @brief The function that the MinProblemSolver calls during optimization
     *
     * @param x input values that the solver will modify during optimization
     * @return double the deltaE computed using the current input
     */
    double calc(const double *x) const;

    /**
     * @brief Get the itteration count object
     *
     * @return int
     */
    int get_itteration_count() { return this->itteration_count; }

  private:
    static int itteration_count; // Keeps track of how many itteration the
                                 // optimization completes
    cv::Mat *opt_in;
    cv::Mat *offeset;
    cv::Mat *M;
    cv::Mat *color_patch_avgs = nullptr;
    cv::Mat *delE_values;
    RefData *ref_data = nullptr;
};

#endif // BEYOND_RGB_BACKEND_COLORMANAGEDCALIBRATOR_H
