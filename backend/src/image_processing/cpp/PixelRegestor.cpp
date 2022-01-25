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


    //Seperating the three channel matrix into multiple single channel arrays
    //We will be using im1Split[2] as registration base

    //im1Split[1] == B
    //im1Split[2] == G
    //im1Split[3] == R

    cv::Mat im1Split[3];
    split(im1, im1Split);

    cv::Mat im2Split[3];
    split(im2, im2Split);

    //Perform three registrations, one on each channel of image 2
    for(int registrationNumber = 1; registrationNumber < 4; registrationNumber--){


        //Warp model
        const int warp_mode = MOTION_EUCLIDEAN;

        cv::Mat warp_matrix;

        if ( warp_mode == MOTION_HOMOGRAPHY ){
            warp_matrix = Mat::eye(3, 3, CV_32F);
        }
        else{
            warp_matrix = Mat::eye(2, 3, CV_32F);
        }

        //Higher iteration number, higher accuracy, higher compute time
        int iterations = 5000;

        double termination_eps = 1e-10;

        TermCriteria criteria(TermCriteria::COUNT+TermCriteria::EPS, iterations, termination_eps);

        //Perform image alignment
        findTransformECC(im1Split[2], im2Split[registrationNumber], warp_matrix, warp_mode, criteria);

        //Storage for registered channel
        cv::Mat aligned;

        if(warp_mode != MOTION_HOMOGRAPHY){
            warpAffine(im2Split[registrationNumber], aligned, warp_matrix, im1Split[2].size(), INTER_LINEAR + WARP_INVERSE_MAP);
        }
        else{
            warpPerspective (im2Split[registrationNumber], aligned, warp_matrix, im1Split[2].size(),INTER_LINEAR + WARP_INVERSE_MAP);
        }
        
        //Overwrite the orginal single channel with the aligned channel
        im2Split[registrationNumber] = aligned;
    }

 
    //Todo 
    //matrix cleanup, ensure bitmap is updated
    





    sleep_for(seconds(1));
}


