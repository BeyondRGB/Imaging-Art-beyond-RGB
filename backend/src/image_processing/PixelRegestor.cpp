#include <image_processing/PixelRegestor.h>
#include <image_util/Image.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_util/image_writer/ImageWriter.hpp>
#include <opencv2/calib3d.hpp>
#include <utils/threading_statics/pixel_regestor_static.hpp>

void PixelRegestor::execute(CommunicationObj *comms, btrgb::ArtObject *images) {
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    std::string output = images->get_output_dir();

    // Grab the image data from the art object
    btrgb::Image *img1 = images->getImage("art1");
    btrgb::Image *img2 = images->getImage("art2");
    btrgb::Image *target1;
    btrgb::Image *target2;

    // thread collection
    std::thread threads[MAX_THREADS];
    std::promise<int> promises[MAX_THREADS];
    std::future<int> futures[MAX_THREADS];

    bool found_target = false;

    try {
        target1 = images->getImage(TARGET(1));
        target2 = images->getImage(TARGET(2));
        found_target = true;
    } catch (std::exception e) {
        comms->send_info(
            "No target was able to be found. Skipping target registration.",
            this->get_name());
        found_target = false;
    }

    int regestration_count = 1;
    if (found_target) {
        regestration_count = 2;
    }

    // threading futures and promises
    std::promise<int> img_registration_promise = std::promise<int>();
    std::future<int> img_registration_future =
        img_registration_promise.get_future();
    std::promise<int> target_registration_promise = std::promise<int>();
    std::future<int> target_registration_future =
        target_registration_promise.get_future();
    std::thread img_registration_thread;
    std::thread target_registration_thread;

    // create a thread for the two image renderers
    img_registration_thread = std::thread(
        btrgb::pixelregestor::apply_regestration, comms, img1, img2, 0,
        regestration_count, output + "img", this->get_name(),
        this->RegistrationFactor, std::move(img_registration_promise));

    if (found_target) {
        target_registration_thread = std::thread(
            btrgb::pixelregestor::apply_regestration, comms, img1, img2, 1,
            regestration_count, "target", this->get_name(),
            this->RegistrationFactor, std::move(target_registration_promise));
    }

    // join and acquire futures (money go UP)
    img_registration_thread.join();
    int matched = img_registration_future.get();
    if (found_target) {
        target_registration_thread.join();
        matched = target_registration_future.get();
    }

    // Add number of matched points to results
    CalibrationResults *results_obj =
        images->get_results_obj(btrgb::ResultType::GENERAL);
    results_obj->store_int(GI_MATCHED_POINTS, matched);
}
