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

    //Grab the image data from the art object
    btrgb::Image* img1 = images->getImage("art1");
    btrgb::Image* img2 = images->getImage("art2");

    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

    cv::Mat Blurred1;
    cv::Mat Blurred2;



    //Amount to blur by 3 seems to be okish needs more testing
    int sigma = 3;
    int ksize = (sigma * 5) | 1;
    //Sharpen factor
    int sharpFactor = 1;


    GaussianBlur(im1, Blurred1, Size(ksize, ksize), sigma, sigma);
    GaussianBlur(im2, Blurred2, Size(ksize, ksize), sigma, sigma);
    
    //Create a mask
    cv::Mat unsharpMask1 = im1 - Blurred1;
    cv::Mat unsharpMask2 = im2 - Blurred2;

    //Apply the mask
    im1 = im1 + sharpFactor * unsharpMask1;
    im2 = im2 + sharpFactor * unsharpMask2;

    images->outputImageAs(btrgb::TIFF, "art1", "Sharp1");
    images->outputImageAs(btrgb::TIFF, "art2", "Sharp2");


    comms->send_progress(0.5, "NoiseReduction");


    //Noise reduction
    //Several different noise reduction algos
    //Using Bilateral Filtering for highest accuracy
    


    cv::Mat filter1;
    cv::Mat filter2;
    try {

        cv::bilateralFilter(im1, filter1, 3, 3.0, 3.0);
        cv::bilateralFilter(im2, filter2, 3, 3.0, 3.0);

        filter1.copyTo(im1);
        filter2.copyTo(im2);

    }
    catch(exception& e)
    {
        std::cout << "An exception occurred. Exception Nr. " << e.what() << '\n';
    }

    comms->send_progress(1, "NoiseReduction");
    //Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "NoiseReduc1");
    images->outputImageAs(btrgb::TIFF, "art2", "NoiseReduc2");


 }