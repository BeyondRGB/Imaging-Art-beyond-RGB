#include <image_util/Image.hpp>
#include <image_util/image_writer/ImageWriter.hpp>
#include <image_processing/PixelRegestor.h>
// #include <../../vcpkg/installed/arm64-osx/include/opencv4/opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void PixelRegestor::execute(CommunicationObj *comms, btrgb::ArtObject *images)
{
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    std::string output = images->get_output_dir();

    //Grab the image data from the art object
    btrgb::Image* img1 = images->getImage("art1");
    btrgb::Image* img2 = images->getImage("art2");
    btrgb::Image* target1;
    btrgb::Image* target2;

    bool found_target = false;

    try{
        target1 = images->getImage(TARGET(1));
        target2 = images->getImage(TARGET(2));
        found_target = true;
    }catch(std::exception e){
        comms->send_error("[Pixel Regestor]", "No target was able to be found.", std::stacktrace::current());
        found_target = false;
    }

    int regestration_count = 1;
    if(found_target){
        regestration_count = 2;
    }

    int matched = this->appy_regestration(comms, img1, img2, 1, regestration_count, output + "img");

    if(found_target){
        matched = this->appy_regestration(comms, target1, target2, 2, regestration_count, output + "target");
    }

    // Add number of matched points to results
    CalibrationResults* results_obj = images->get_results_obj(btrgb::ResultType::GENERAL);
    results_obj->store_int(GI_MATCHED_POINTS, matched);

    //Outputs TIFFs for each image group for after this step, temporary
    // images->outputImageAs(btrgb::TIFF, "art1", "art1_rgstr");
    // images->outputImageAs(btrgb::TIFF, "art2", "art2_rgstr");

}

int PixelRegestor::appy_regestration(CommunicationObj* comms, btrgb::Image *img1, btrgb::Image *img2, int cycle, int cycle_count, std::string output){
    
    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

     //Check that there is actual data in them
    if (!im1.data || !im2.data)
    {
        return 0;
    }

    int MAX_FEATURES;
    const float GOOD_MATCH_PERCENT = 0.25f;

    //More feature slower but better
    if (RegistrationFactor == "L") {
        MAX_FEATURES = 400;
    }
    else if (RegistrationFactor == "M") {
        MAX_FEATURES = 600;
    }
    else if (RegistrationFactor == "H") {
        MAX_FEATURES = 1000;
    }
    else {
        MAX_FEATURES = 600;
    }

    float prog = 0;

    // Registered image will be resotred in imReg.
    // The estimated homography will be stored in h.
    cv::Mat im2reg, h;

    cv::Mat im18;
    cv::Mat im28;

    //Make a copy of the data in 8bit format to allow orb dection
    prog = this->calc_progress(0.10, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, this->get_name());
    im1.convertTo(im18, CV_8UC3, 255);
    im2.convertTo(im28, CV_8UC3, 255);

    cv::Mat im18gray, im28gray;


    //Todo add check for memory
    cv::cvtColor(im18, im18gray, cv::COLOR_RGB2GRAY);
    cv::cvtColor(im28, im28gray, cv::COLOR_RGB2GRAY);



    // Variables to store keypoints and descriptors
    std::vector<KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // Detect ORB features and compute descriptors.
    prog = this->calc_progress(0.25, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, this->get_name());
    Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im18gray, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(im28gray, Mat(), keypoints2, descriptors2);

    // Match features.
    prog = this->calc_progress(0.30, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, this->get_name());
    std::vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());

    // Sort matches by score
    std::sort(matches.begin(), matches.end());

    // Remove not so good matches
    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    // Extract location of good matches
    std::vector<Point2f> points1;
    std::vector<Point2f> points2;
    std::vector<DMatch> good_matches;

    // Clean out obviously bad mathces
    for (size_t i = 0; i < matches.size(); i++)
    {
        const int threshold = 15;
        Point2f p1 = keypoints1[matches[i].queryIdx].pt;
        Point2f p2 = keypoints2[matches[i].trainIdx].pt;

        if (abs(p2.x - p1.x) < threshold && abs(p2.y - p1.y) < threshold)
        {
            points1.push_back(p1);
            points2.push_back(p2);
            good_matches.push_back(matches[i]);
            // cout << "(" << p1.x << "," << p1.y << ") <=> (" << p2.x << "," << p2.y << ")" << std::endl;
        }
    }

    // Draw top matches and send to front end
    cv::Mat imMatches;
    drawMatches(im18, keypoints1, im28, keypoints2, good_matches, imMatches);
    cv::Mat imS;
    cv::resize(imMatches, imS, cv::Size(), 0.25, 0.25);
    //cv::imwrite("matches.tiff", imMatches);
    cv::Mat matchfloat;
    imMatches.convertTo(matchfloat, CV_32FC3, 1.0 / 0xFF);
    std::unique_ptr<btrgb::Image> btrgb_matches(new btrgb::Image("matches"));
    btrgb_matches->initImage(matchfloat);
    comms->send_binary(btrgb_matches.get(), btrgb::FULL);

    btrgb::ImageWriter(btrgb::TIFF).write(btrgb_matches.get(), output + "_matches"); // Output matches as a TIFF in the output folder

    btrgb_matches.reset(nullptr);

    // Find homography
    prog = this->calc_progress(0.75, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, this->get_name());
    h = findHomography(points2, points1, RANSAC);

    // Use homography to warp image
    //First param is image to be aligned, 2nd is storage for aliagned image, third is homography, fourth is size of orginal img
    prog = this->calc_progress(0.85, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, this->get_name());
    warpPerspective(im2, im2reg, h, im1.size());

    // Copy image
    im2reg.copyTo(im2);

    // Print estimated homography, prolly want to store this somewhere for report?
    cout << "Estimated homography : \n" << h;

    prog = this->calc_progress(1, (float)cycle, (float)cycle_count);
    comms->send_progress(prog, this->get_name());
    return good_matches.size();
}

float PixelRegestor::calc_progress(float progress, float cycle, float cycle_count){
    float offset = (cycle - 1) / cycle_count;
    return progress / cycle_count + offset;
}
