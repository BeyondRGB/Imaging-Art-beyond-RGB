#include <future> 

#include <utils/threading_statics/pixel_regestor_static.hpp>
#include <image_processing/PixelRegestor.h>
#include <image_util/Image.hpp>
#include <image_util/image_writer/ImageWriter.hpp>
#include <opencv2/calib3d.hpp>

static std::mutex comms_mutex;

void btrgb::pixelregestor::apply_regestration(CommunicationObj *comms,
                                            btrgb::Image *img1, btrgb::Image *img2, cv::Mat matchfloat,
                                            int cycle, int cycle_count,
                                            int row, int col, int subset_height, int subset_width,
                                            std::string output, std::string name,
                                            std::string RegistrationFactor, std::promise<int> && p) {
    // takes a subset of the image to process
    cv::Mat im1 = img1->getMat()(cv::Rect(col, row, subset_width, subset_height));
    // takes a subset of the image to process
    cv::Mat im2 = img2->getMat()(cv::Rect(col, row, subset_width, subset_height));
    // takes a subset of the image to process
    cv::Mat matchfloat_subset = matchfloat(cv::Rect(col, row, subset_width * 2, subset_height));

    // Check that there is actual data in them
    if (!im1.data || !im2.data) {
        p.set_value(0);
        return;
    }

    int MAX_FEATURES;
    const float GOOD_MATCH_PERCENT = 0.25f;

    // More feature slower but better
    if (RegistrationFactor == "L") {
        MAX_FEATURES = 400;
    } else if (RegistrationFactor == "M") {
        MAX_FEATURES = 600;
    } else if (RegistrationFactor == "H") {
        MAX_FEATURES = 1000;
    } else {
        MAX_FEATURES = 600;
    }

    float prog = 0;

    // Registered image will be resotred in imReg.
    // The estimated homography will be stored in h.
    cv::Mat im2reg, h;

    cv::Mat im18;
    cv::Mat im28;

    // Make a copy of the data in 8bit format to allow orb dection
    comms_mutex.lock();
    prog = btrgb::pixelregestor::calc_progress(0.10, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, name);
    comms_mutex.unlock();
    im1.convertTo(im18, CV_8UC3, 255);
    im2.convertTo(im28, CV_8UC3, 255);

    cv::Mat im18gray, im28gray;

    // Todo add check for memory
    cv::cvtColor(im18, im18gray, cv::COLOR_RGB2GRAY);
    cv::cvtColor(im28, im28gray, cv::COLOR_RGB2GRAY);

    // Variables to store keypoints and descriptors
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // Detect ORB features and compute descriptors.
    comms_mutex.lock();
    prog = btrgb::pixelregestor::calc_progress(0.15, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, name);
    comms_mutex.unlock();
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im18gray, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(im28gray, cv::Mat(), keypoints2, descriptors2);

    // Match features.
    comms_mutex.lock();
    prog = btrgb::pixelregestor::calc_progress(0.05, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, name);
    comms_mutex.unlock();
    std::vector<cv::DMatch> matches;
    cv::Ptr<cv::DescriptorMatcher> matcher =
        cv::DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, cv::Mat());

    // Sort matches by score
    std::sort(matches.begin(), matches.end());

    // Remove not so good matches
    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    // Extract location of good matches
    std::vector<cv::Point2f> points1;
    std::vector<cv::Point2f> points2;
    std::vector<cv::DMatch> good_matches;

    // Clean out obviously bad mathces
    for (size_t i = 0; i < matches.size(); i++) {
        const int threshold = 15;
        cv::Point2f p1 = keypoints1[matches[i].queryIdx].pt;
        cv::Point2f p2 = keypoints2[matches[i].trainIdx].pt;

        if (abs(p2.x - p1.x) < threshold && abs(p2.y - p1.y) < threshold) {
            points1.push_back(p1);
            points2.push_back(p2);
            good_matches.push_back(matches[i]); 
        }
    }

    // Draw top matches and send to front end
    cv::Mat imMatches;
    drawMatches(im18, keypoints1, im28, keypoints2, good_matches, imMatches);
    cv::Mat imS;
    cv::resize(imMatches, imS, cv::Size(), 0.25, 0.25);
    // cv::imwrite("matches.tiff", imMatches); 
    imMatches.convertTo(matchfloat_subset, CV_32FC3, 1.0 / 0xFF); 

    // Find homography
    comms_mutex.lock();
    prog = btrgb::pixelregestor::calc_progress(0.45, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, name);
    comms_mutex.unlock();
    h = cv::findHomography(points2, points1, cv::RANSAC);

    // Use homography to warp image
    // First param is image to be aligned, 2nd is storage for aliagned image,
    // third is homography, fourth is size of orginal img
    comms_mutex.lock();
    prog = btrgb::pixelregestor::calc_progress(0.1, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, name);
    comms_mutex.unlock();
    warpPerspective(im2, im2reg, h, im1.size());

    // Copy image
    im2reg.copyTo(im2);

    // Print estimated homography, prolly want to store this somewhere for
    // report?
    std::cout << "Estimated homography : \n" << h;

    
    comms_mutex.lock();
    prog = btrgb::pixelregestor::calc_progress(.15, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, name);
    comms_mutex.unlock();
    p.set_value(good_matches.size());
}

float btrgb::pixelregestor::calc_progress(float progress, float cycle,
                                   float cycle_count) {
    static double prog = 0;
    // figure out 
    float overall_perc = (1 / cycle_count) * progress;
    prog += overall_perc;
    return prog;
}
// float PixelRegestor::calc_progress(float progress, float cycle,
//                                    float cycle_count) {
//     float offset = (cycle - 1) / cycle_count;
//     return progress / cycle_count + offset;
// }
