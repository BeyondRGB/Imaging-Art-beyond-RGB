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

    cout << "Filter Time";

    //Grab the image data from the art object
    btrgb::Image* img1 = images->getImage("art1");
    btrgb::Image* img2 = images->getImage("art2");

    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

    cv::FileStorage file("AfterFlat.yml", cv::FileStorage::WRITE);
    file << "matName" << im1;

    cv::Mat Hblurred1;
    cv::Mat Hblurred2;

    //High Frequency Kernel larger sigma = more sharp
    //Low = 0.5  Med = 1  High = 1.5

    string sharpen = images->get_sharpen_type();

    //Sharpen value passed in 
    if (sharpen == "L" || sharpen == "M" || sharpen == "H") {
        int sigma;
        if (sharpen == "L") {
            sigma = 0.5;
        }
        else if (sharpen == "M") {
            sigma = 1;
        }
        else {
            sigma = 1.5;
        }
        int ksize = (sigma * 5) | 1;

        //Sharpen Factor
        int HsharpFactor = 1;

        //High Freq Blur
        GaussianBlur(im1, Hblurred1, Size(ksize, ksize), sigma, sigma);
        GaussianBlur(im2, Hblurred2, Size(ksize, ksize), sigma, sigma);

        //Create high freq mask
        cv::Mat unsharpMask1 = im1 - Hblurred1;
        cv::Mat unsharpMask2 = im2 - Hblurred2;

        //Apply high freq mask
        im1 = im1 + HsharpFactor * unsharpMask1;
        im2 = im2 + HsharpFactor * unsharpMask2;

        //Output sharpened image
        images->outputImageAs(btrgb::TIFF, "art1", "Sharp1");
        images->outputImageAs(btrgb::TIFF, "art2", "Sharp2");

        comms->send_progress(0.5, "Sharpening");
        comms->send_progress(1, "Sharpening");
    }
    //Sharpen value default 0 or invalid value, don't sharpen
    else {
        comms->send_progress(1, "Skipping Sharpening");
    }
 }