//
// Created by ThinkPad41 on 10/10/2021.
//
#include "ImageUtil/Image.hpp"
#include "../header/PixelRegestor.h"
#include <opencv2/opencv.hpp>

using namespace cv;



void PixelRegestor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Pixel Registration");

    //Grab the image data from the art object
    btrgb::image* img1 = images->getImage("art1");
    btrgb::image* img2 = images->getImage("art2");

    int ImageARows = img1->width();
    int ImageACols = img1->height();
    void* dataA = img1-> bitmap();
    
    int ImageBRows = img2->width();
    int ImageBCols = img2->height();
    void* dataB = img2-> bitmap();

    int type = CV_16UC3;

    //Convert the two bitmaps from ArtObject into OpenCV matrix format

    cv::Mat im1(ImageARows, ImageACols, type, dataA);
    cv::Mat im2(ImageBRows, ImageBCols, type, dataB);




   
    //Check that there is actual data in them
    if(!im1.data || !im2.data)
    {
       func("Error: No data in OpenCv Matrixs");
        return;
    }


    //Convert to 32bit floating point
    cv::Mat im1_32f;
    cv::Mat im2_32f;

    im1.convertTo(im1_32f, CV_32FC3);
    im2.convertTo(im2_32f, CV_32FC3);


    
    //Seperating the three channel matrix into multiple single channel arrays
    //We will be using im1Split[2] as registration base

    //im1Split[0] == B
    //im1Split[1] == G
    //im1Split[2] == R

    cv::Mat im1Split[3];
    cv::split(im1_32f, im1Split);

    cv::Mat im2Split[3];
    cv::split(im2_32f, im2Split);

    std::cout << "Register start \n";

    //Warp model
    const int warp_mode = MOTION_EUCLIDEAN;

    cv::Mat warp_matrix;

    std::cout << "Setting up warp matrix \n";

    if (warp_mode == MOTION_HOMOGRAPHY) {
        warp_matrix = Mat::eye(3, 3, CV_32F);
    }
    else {
        warp_matrix = Mat::eye(2, 3, CV_32F);
    }
  
    std::cout << "Setting criteria \n";
    //Higher iteration number, higher accuracy, higher compute time

    //Takes about 8-10 seconds per iteration, recommend keeping low till testing is done.
    int iterations = 6;

    double termination_eps = 1e-10;

    TermCriteria criteria(TermCriteria::COUNT + TermCriteria::EPS, iterations, termination_eps);

   

    std::cout << "Estimating warp matrix \n";
    //Perform image alignment
    findTransformECC(im1Split[1], im2Split[1], warp_matrix, warp_mode, criteria);


    //Perform three registrations, currently reusing warp matrix from Im1[1] and Im2[1]

    for (int registrationNumber = 2; registrationNumber >= 0; registrationNumber--) {

        std::cout << "Warping image \n";
        std::cout << registrationNumber;

        //Storage for registered channel
        cv::Mat aligned;

        if (warp_mode != MOTION_HOMOGRAPHY) {
            warpAffine(im2Split[registrationNumber], aligned, warp_matrix, im1Split[1].size(), INTER_LINEAR + WARP_INVERSE_MAP);
        }
        else {
            warpPerspective(im2Split[registrationNumber], aligned, warp_matrix, im1Split[1].size(), INTER_LINEAR + WARP_INVERSE_MAP);
        }
        std::cout << "Done \n";
        //Overwrite the orginal single channel with the aligned channel
        im2Split[registrationNumber] = aligned;

    }

 
    //Merging 3 split channels back into 1 matrix
    cv::merge(im2Split, 3, im2_32f);

    //Convert from 32 bit floating to 16 bit
    im2_32f.convertTo(im2, CV_16UC3);

    //Outputs TIFFs for each image group for after this step, temporary
    img1->setFilename("RegistrationOut1");
    img2->setFilename("RegistrationOut2");
    images->outputImageAsTIFF("art1");
    images->outputImageAsTIFF("art2");

}


