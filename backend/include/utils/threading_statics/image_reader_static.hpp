#ifndef IMAGE_READER_STATIC_H
#define IMAGE_READER_STATIC_H

#include <image_util/ImageLoader.hpp>

namespace btrgb {
namespace imagereader {

/**
 * @brief allows for asynchronous updates of the front end by threads
 *  utilizing mutexes to control when a thread can report its completion
 * @param comms pointer to the comms obj
 * @param comms_mutex the lock to request access for
 * @param name name of the process to update
 * @param total_images this value is only normally known to the image reader, is
 *  provided to allow proper statistics of progress
 */
void update_reading_progress(CommunicationObj *comms, std::mutex *comms_mutex,
                             std::string name, int total_images);

/**
 * @brief static method to allow image loading to be threaded
 * @param l the ImageLoader to begin execution on
 */
void load_image(ImageLoader *l);

} // namespace imagereader
} // namespace btrgb

#endif // IMAGE_READER_STATIC_H