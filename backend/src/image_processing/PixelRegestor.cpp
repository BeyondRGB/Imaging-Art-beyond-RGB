#include <image_processing/PixelRegestor.h>
#include <image_util/Image.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_util/image_writer/ImageWriter.hpp>
#include <opencv2/calib3d.hpp>
#include <utils/threading_statics/pixel_regestor_static.hpp>

void PixelRegestor::send_image(CommunicationObj *comms, std::string output, cv::Mat matchfloat) {
    std::unique_ptr<btrgb::Image> btrgb_matches(new btrgb::Image("matches"));
    btrgb_matches->initImage(matchfloat);
    comms->send_binary(btrgb_matches.get(), btrgb::FULL);
    btrgb::ImageWriter(btrgb::TIFF)
        .write(btrgb_matches.get(),
                output +
                    "_matches"); // Output matches as a TIFF in the output folder

    btrgb_matches.reset(nullptr);
}

void PixelRegestor::execute(CommunicationObj *comms, btrgb::ArtObject *images) {
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    std::string output = images->get_output_dir();

    // Grab the image data from the art object
    btrgb::Image *img1 = images->getImage("art1");
    btrgb::Image *img2 = images->getImage("art2");
    btrgb::Image *target1;
    btrgb::Image *target2;
    cv::Mat matchfloat = cv::Mat::zeros(img1->height(), img1->width() * 2, CV_32FC3);

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
    int matched = 0;
    if (found_target) {
        regestration_count = 2;
    }
    // multiplied by number of threads we will be creating, keeps 
    //  comms updates the same thank god
    regestration_count *= MAX_THREADS;

    // dims
    int width = img1->width();
    int height = img1->height();
    int chunk_height = height / MAX_THREADS;
    // chunk out the process
    for (int i = 0; i < MAX_THREADS; i++)
    {
        promises[i] = std::promise<int>();
        futures[i] = promises[i].get_future();
        threads[i] = 
            std::thread(
                btrgb::pixelregestor::apply_regestration,
                comms, img1, img2, matchfloat, i + 1, regestration_count,
                (i * chunk_height), 0, 
                (i == MAX_THREADS - 1) ? height - (chunk_height * i) : chunk_height, width,
                output + "img", this->get_name(), this->RegistrationFactor,
                std::move(promises[i])
            );
    }
    for (int i = 0; i < MAX_THREADS; i++)
    {
        threads[i].join();
        matched += futures[i].get();
    }
    // send and save the image
    this->send_image(comms, output + "img", matchfloat);

    if (found_target) {
        // dims
        matched = 0;
        width = target1->width();
        height = target1->height();
        chunk_height = height / MAX_THREADS;
        // chunk out the process
        for (int i = 0; i < MAX_THREADS; i++)
        {
            promises[i] = std::promise<int>();
            futures[i] = promises[i].get_future();
            threads[i] = 
                std::thread(
                    btrgb::pixelregestor::apply_regestration,
                    comms, target1, target2, matchfloat, i + MAX_THREADS + 1, regestration_count,
                    (i * chunk_height), 0, 
                    (i == MAX_THREADS - 1) ? height - (chunk_height * i) : chunk_height, width,
                    output + "target", this->get_name(), this->RegistrationFactor,
                    std::move(promises[i])
                );
        }
        for (int i = 0; i < MAX_THREADS; i++)
        {
            threads[i].join();
            matched += futures[i].get();
        }
        // send and save the image
        this->send_image(comms, output + "target", matchfloat);
    }

    // Add number of matched points to results
    CalibrationResults *results_obj =
        images->get_results_obj(btrgb::ResultType::GENERAL);
    results_obj->store_int(GI_MATCHED_POINTS, matched);
}
