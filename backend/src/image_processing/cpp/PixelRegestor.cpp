//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PixelRegestor.h"
#include <opencv2/opencv.hpp>

using namespace cv;



void PixelRegestor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Pixel Registration");

    //Initalize two openCv matrixs one for each bitmap
    cv::Mat im1;
    cv::Mat im2;


    //Convert the two bitmaps from ArtObject into OpenCV matrix format


    //Check that there is actual data in them
    if(!im1.data || !im2.data)
    {
       func("Error: No data in OpenCv Matrixs");
        return;
    }


    //Seperating the three channel matrix into multiple single channel arrays
    //We will be using im1rbgchannels[2] as registration base
    cv::Mat img1rbgchannels[3];
    cv:split(im1, img1rbgchannels);

    cv::Mat img2rbgchannels[3];
    cv:split(im2, img2rbgchannels);

    //Warp model
    const int warp_mode = MOTION_EUCLIDEAN;



    sleep_for(seconds(1));
}


