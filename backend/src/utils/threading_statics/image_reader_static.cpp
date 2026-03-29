#include <mutex>

#include <utils/threading_statics/image_reader_static.hpp>

void btrgb::imagereader::update_reading_progress(CommunicationObj *comms, std::mutex *comms_mutex, std::string name, int total_images)
{
    // starts at 0, slowly increments
    static int count = 0;

    comms_mutex->lock();
    count++;
    comms->send_progress(count / total_images, name);
    comms_mutex->unlock();
}

void btrgb::imagereader::load_image(ImageLoader *l)
{
    l->load_image();
}
