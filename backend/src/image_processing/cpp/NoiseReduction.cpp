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
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    //Grab the image data from the art object
    btrgb::Image* img1 = images->getImage("art1");
    btrgb::Image* img2 = images->getImage("art2");

    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

    cv::Mat Hblurred1;
    cv::Mat Hblurred2;

    //High Frequency Kernel larger sigma = more sharp
    //Low = 0.5  Med = 1  High = 1.5
    std::cout << SharpenFactor;
    int sigma = 0.5;

    //Sharpen value passed in 
    if (SharpenFactor == "L") {
        sigma = 0.5;
    }
    else if (SharpenFactor == "M") {
        sigma = 1;
    }
    else if (SharpenFactor == "H"){
        sigma = 1.5;
    }
    
    //Sharpen Factor
    int HsharpFactor = 1;

    int ksize = (sigma * 5) | 1;

    //High Freq Blur
    GaussianBlur(im1, Hblurred1, Size(ksize, ksize), sigma, sigma);
    GaussianBlur(im2, Hblurred2, Size(ksize, ksize), sigma, sigma);
    
    //Create high freq mask
    cv::Mat unsharpMask1 = im1 - Hblurred1;
    cv::Mat unsharpMask2 = im2 - Hblurred2;

    //Apply just high freq mask
    im1 = im1 + HsharpFactor * unsharpMask1;
    im2 = im2 + HsharpFactor * unsharpMask2;

    images->outputImageAs(btrgb::TIFF, "art1", "Sharp1");
    images->outputImageAs(btrgb::TIFF, "art2", "Sharp2");

    comms->send_progress(0, this->get_name());

    //Noise reduction
    //Using Bilateral Filtering for highest accuracy
    //Filter can't run in place must copy to temp matrixs
    cv::Mat filter1;
    cv::Mat filter2;
    int noiseReducKernel = 2;
    cv::bilateralFilter(im1, filter1, noiseReducKernel, noiseReducKernel * 2, noiseReducKernel / 2);
    comms->send_progress(0, this->get_name());

    cv::bilateralFilter(im2, filter2, noiseReducKernel, noiseReducKernel * 2, noiseReducKernel / 2);

    //Copy back to art object
    filter1.copyTo(im1);
    filter2.copyTo(im2);

    comms->send_progress(0, this->get_name());
    //Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "NoiseReduc1");
    images->outputImageAs(btrgb::TIFF, "art2", "NoiseReduc2");
     
 }