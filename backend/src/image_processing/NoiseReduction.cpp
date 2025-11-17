//
// Created by ThinkPad41 on 10/10/2021.
//
#include <image_util/Image.hpp>
#include <image_processing/NoiseReduction.h>

using namespace cv;
#include <exception>
using namespace std;

void NoiseReduction::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    cout << "Filter Time";

    //Grab the image data from the art object
    btrgb::Image* img1 = images->getImage("art1");
    btrgb::Image* img2 = images->getImage("art2");
    btrgb::Image* target1;
    btrgb::Image* target2;
    bool targets_found = false;

    try {
        target1 = images->getImage(TARGET(1));
        target2 = images->getImage(TARGET(2));
        targets_found = true;
    }
    catch (std::exception e) {
        comms->send_error("[Noise Reduction]", "No target was able to be found.", cpptrace::generate_trace());
        targets_found = false;
    }

    this->apply_filter(img1, img2);

    if (targets_found) {
        this->apply_filter(target1, target2);
    }

    comms->send_progress(1, this->get_name());
    //Output sharpened image
    //images->outputImageAs(btrgb::TIFF, "art1", "Sharp1");
    //images->outputImageAs(btrgb::TIFF, "art2", "Sharp2");

}

void NoiseReduction::apply_filter(btrgb::Image* img1, btrgb::Image* img2) {
    cv::Mat im1 = img1->getMat();
    cv::Mat im2 = img2->getMat();

    int ksize = 0;
    //Sharpen value passed in 
    if (SharpenFactor == "L") {
        ksize = 1;
    }
    else if (SharpenFactor == "M") {
        ksize = 3;
    }
    else if (SharpenFactor == "H") {
        ksize = 5;
    }

    cv::Mat Hblurred1;
    cv::Mat Hblurred2;

    //High Frequency Kernel larger sigma = more sharp
    //Low = 0.5  Med = 1  High = 1.5

    //Sharpen value passed in 

    //Sharpen Factor
    int HsharpFactor = 1;

    //High Freq Blur
    GaussianBlur(im1, Hblurred1, Size(ksize, ksize), 1, 1);
    GaussianBlur(im2, Hblurred2, Size(ksize, ksize), 1, 1);

    //Create high freq mask
    cv::Mat unsharpMask1 = im1 - Hblurred1;
    cv::Mat unsharpMask2 = im2 - Hblurred2;

    //Apply high freq mask
    im1 = im1 + HsharpFactor * unsharpMask1;
    im2 = im2 + HsharpFactor * unsharpMask2;

    //Noise reduction
    //Using Bilateral Filtering for highest accuracy
    //Filter can't run in place must copy to temp matrixs
    cv::Mat filter1;
    cv::Mat filter2;
    int noiseReducKernel = 2;
    cv::bilateralFilter(im1, filter1, noiseReducKernel, noiseReducKernel * 2, noiseReducKernel / 2);
    cv::bilateralFilter(im2, filter2, noiseReducKernel, noiseReducKernel * 2, noiseReducKernel / 2);

    //Copy back to art object
    filter1.copyTo(im1);
    filter2.copyTo(im2);
}
