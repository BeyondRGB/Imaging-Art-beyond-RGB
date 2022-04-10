#include "ImageUtil/Image.hpp"
#include "../header/PixelRegestor.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void PixelRegestor::execute(CommunicationObj *comms, btrgb::ArtObject *images)
{
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    const int MAX_FEATURES = 500;
    const float GOOD_MATCH_PERCENT = 0.12f;

    // Grab the image data from the art object
    btrgb::Image *img1 = images->getImage("art1");
    btrgb::Image *img2 = images->getImage("art2");

    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

    // Check that there is actual data in them
    if (!im1.data || !im2.data)
    {
        return;
    }

    // Registered image will be resotred in imReg.
    // The estimated homography will be stored in h.
    cv::Mat im2reg, h;

    cv::Mat im18;
    cv::Mat im28;

    // Make a copy of the data in 8bit format to allow orb dection
    comms->send_progress(0.10, this->get_name());
    im1.convertTo(im18, CV_8UC3, 255);
    im2.convertTo(im28, CV_8UC3, 255);

    cv::Mat im18gray, im28gray;

    cv::cvtColor(im18, im18gray, cv::COLOR_RGB2GRAY);
    cv::cvtColor(im28, im28gray, cv::COLOR_RGB2GRAY);

    // Variables to store keypoints and descriptors
    std::vector<KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // Detect ORB features and compute descriptors.
    comms->send_progress(0.25, this->get_name());
    Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im18gray, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(im28gray, Mat(), keypoints2, descriptors2);

    // Match features.
    comms->send_progress(0.30, this->get_name());
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
            cout << "(" << p1.x << "," << p1.y << ") <=> (" << p2.x << "," << p2.y << ")" << std::endl;
        }
    }

    // Draw top matches and send to front end
    cv::Mat imMatches;
    drawMatches(im18, keypoints1, im28, keypoints2, good_matches, imMatches);
    cv::Mat imS;
    cv::resize(imMatches, imS, cv::Size(), 0.25, 0.25);
    // cv::imwrite("matches.tiff", imMatches);
    cv::Mat matchfloat;
    imMatches.convertTo(matchfloat, CV_32FC3, 1.0 / 0xFF);
    btrgb::Image *btrgb_matches(new btrgb::Image("matches"));
    btrgb_matches->initImage(matchfloat);
    comms->send_binary(btrgb_matches, btrgb::FULL);
    images->setImage("matches", btrgb_matches);
    images->outputImageAs(btrgb::PNG, "matches");
    images->deleteImage("matches");

    // Find homography
    comms->send_progress(0.75, this->get_name());
    h = findHomography(points2, points1, RANSAC);

    // Use homography to warp image
    // First param is image to be aligned, 2nd is storage for aliagned image, third is homography, fourth is size of orginal img
    comms->send_progress(0.85, this->get_name());
    warpPerspective(im2, im2reg, h, im1.size());

    // Copy image
    im2reg.copyTo(im2);

    // Print estimated homography, prolly want to store this somewhere for report?
    cout << "Estimated homography : \n"
         << h;
    comms->send_progress(1, this->get_name());

    // Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "art1_rgstr");
    images->outputImageAs(btrgb::TIFF, "art2", "art2_rgstr");
}
