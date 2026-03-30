#ifndef PIXEL_REGESTOR_STATIC_H
#define PIXEL_REGESTOR_STATIC_H

#include <image_util/ImageLoader.hpp>

namespace btrgb {
namespace pixelregestor {

/**
 * @brief applies the regestration algorithm to find good matches
 * @param comms the communications object to send progress updates to
 * @param img1 the first base image
 * @param img2 the second base image
 * @param cycle the current cycle of regestration
 * @param cycle_count total number of cycles in regestration
 * @param name name of the process to update in comms
 * @param RegistrationFactor the factor mode of registration
 * @param p future to set and return value with
 * @returns int number of good matches
 */
void apply_regestration(CommunicationObj *comms, btrgb::Image *img1,
                        btrgb::Image *img2, int cycle, int cycle_count,
                        std::string output, std::string name,
                        std::string RegistrationFactor, std::promise<int> &&p);

/**
 * @brief calculates the progress of regestration based on total number of
 *  regestrations to be done
 * @param progress the current progress
 * @param cycle the current cycle
 * @param cycle_count the total number of cycles
 * @returns float percentage
 */
float calc_progress(float progress, float cycle, float cycle_count);

} // namespace pixelregestor
} // namespace btrgb

#endif // PIXEL_REGESTOR_STATIC_H