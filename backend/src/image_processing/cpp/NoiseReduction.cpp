//
// Created by ThinkPad41 on 10/10/2021.
//
#include "ImageUtil/Image.hpp"
#include "../header/NoiseReduction.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
#include <exception>
using namespace std;

void NoiseReduction::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("", "NoiseReduction");
    comms->send_progress(0, "NoiseReduction");

    cout << "Filter Time-------------------------------------------------------------------------------------------------------------------------------------\n";

    //Grab the image data from the art object
    btrgb::Image* img1 = images->getImage("art1");
    btrgb::Image* img2 = images->getImage("art2");

    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

    cv::FileStorage file("inf.yml", cv::FileStorage::WRITE);
    file << "matName" << im1;

    cv::Mat Hblurred1;
    cv::Mat Hblurred2;

    cv::Mat Lblurred1;
    cv::Mat Lblurred2;

    //High Frequency Kernel
    int sigma = 2;
    int ksize = (sigma * 5) | 1;

    //Low Frequency Kernel
    //int sigmaL = 1;
    //int ksizeL = 1.2;

    //Sharpen Factor
    int HsharpFactor = 1;
    //Noise Factor
    //int LsharpFactor = 1;

    //High Freq Blur
    GaussianBlur(im1, Hblurred1, Size(ksize, ksize), sigma, sigma);
    GaussianBlur(im2, Hblurred2, Size(ksize, ksize), sigma, sigma);
    
    //Low Freq Blur
    //GaussianBlur(im1, Lblurred1, Size(ksizeL, ksizeL), sigmaL, sigmaL);
    //GaussianBlur(im2, Lblurred2, Size(ksizeL, ksizeL), sigmaL, sigmaL);

    //Create high freq mask
    cv::Mat unsharpMask1 = im1 - Hblurred1;
    cv::Mat unsharpMask2 = im2 - Hblurred2;

    //Create low freq mask
    //cv::Mat unNoiseMask1 = im1 - Lblurred1;
    //cv::Mat unNoiseMask2 = im2 - Lblurred2;

    //Apply just high freq mask
    im1 = im1 + HsharpFactor * unsharpMask1;
    im2 = im2 + HsharpFactor * unsharpMask2;

    //Apply both masks
    //im1 = im1 + HsharpFactor * unsharpMask1 - LsharpFactor * unNoiseMask1;
    //im1 = im2 + HsharpFactor * unsharpMask2 - LsharpFactor * unNoiseMask2;


    images->outputImageAs(btrgb::TIFF, "art1", "Sharp1");
    images->outputImageAs(btrgb::TIFF, "art2", "Sharp2");


    comms->send_progress(0.5, "NoiseReduction");


    //Noise reduction
    //Several different noise reduction algos
    //Using Bilateral Filtering for highest accuracy
    cv::Mat filter1;
    cv::Mat filter2;
    cout << "Filter 1-------------------------------------------------------------------------------------------------------------------------------------\n";

    cv::bilateralFilter(im1, filter1, 2, 3, 3);
    cout << "Filter 2-------------------------------------------------------------------------------------------------------------------------------------\n";

    cv::bilateralFilter(im2, filter2, 2, 3, 3);

    filter1.copyTo(im1);
    filter2.copyTo(im2);

    comms->send_progress(1, "NoiseReduction");
    //Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "NoiseReduc1");
    images->outputImageAs(btrgb::TIFF, "art2", "NoiseReduc2");
     
 }