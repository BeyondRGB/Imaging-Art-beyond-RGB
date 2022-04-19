#include "../header/FlatFieldor.h"
#include <iostream>
#include <boost/range/irange.hpp>

void FlatFieldor::execute(CommunicationObj* comms, btrgb::ArtObject* images)
{
    btrgb::Image* art1;
    btrgb::Image* art2;
    btrgb::Image* white1;
    btrgb::Image* white2;
    btrgb::Image* dark1;
    btrgb::Image* dark2;
    btrgb::Image* target1;
    btrgb::Image* target2;
    RefData* reference;

    bool target_found = false;


    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    // Pull the images needed out of the Art Object
    try
    {
        art1 = images->getImage("art1");
        art2 = images->getImage("art2");
        white1 = images->getImage("white1");
        dark1 = images->getImage("dark1");
        white2 = images->getImage("white2");
        dark2 = images->getImage("dark2");
        reference = images->get_refrence_data();
        try {
            target1 = images->getImage(TARGET(1));
            target2 = images->getImage(TARGET(2));
            target_found = true;
        }
        catch (const btrgb::ArtObj_ImageDoesNotExist& e){

            target_found = false;
        }

    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e)
    {
        throw ImgProcessingComponent::error(e.what(), this->get_name());
    }

    // Set up variables for the overall size of all the images, they are all the same size
    int height = art1->height();
    int width = art1->width();
    int channels = art1->channels();

    ColorTarget target = images->get_target(TARGET(1), btrgb::TargetType::GENERAL_TARGET);


    //Col and Row of the white patch on the target
    int whiteRow = target.get_white_row();
    int whiteCol = target.get_white_col();

    //Collecting the y Value from the reference data
    double yVal = reference->get_y(whiteRow, whiteCol);

    //Getting average patch values for white and art images channel two
    float patAvg = target.get_patch_avg(whiteRow, whiteCol, 1);
    float whiteAvg = images->get_target("white1", btrgb::TargetType::GENERAL_TARGET).get_patch_avg(whiteRow, whiteCol, 1);

    //Calculate w value and complete the pixel operation with set w value
    wCalc(patAvg, whiteAvg, yVal);


    //Perform flatfielding and dead pixel cleanup


    //Image set 1-----------------------------------------------------------
    btrgb::Image* art1copy = new btrgb::Image("art1copy");
    cv::Mat copy = btrgb::Image::copyMatConvertDepth(art1->getMat(), CV_32F);
    art1copy->initImage(copy);

    pixelOperation(height, width, channels, art1, white1, dark1, art1copy);
    comms->send_progress(0.5, this->get_name());

    delete art1copy;


    //Image set 2-----------------------------------------------------------
    btrgb::Image* art2copy = new btrgb::Image("art2copy");
    cv::Mat copy2 = btrgb::Image::copyMatConvertDepth(art2->getMat(), CV_32F);
    art2copy->initImage(copy2);

    pixelOperation(height, width, channels, art2, white2, dark2, art2copy);
    comms->send_progress(1, this->get_name());

    delete art2copy;


    //If there are seperate targets needs to copy to do dead pixel detection
    if (target_found) {


        height = target1->height();
        width = target1->width();
        channels = target1->channels();


        //Copy and delete instantly after operation
        btrgb::Image* target1copy = new btrgb::Image("target1copy");
        cv::Mat tcopy = btrgb::Image::copyMatConvertDepth(target1->getMat(), CV_32F);
        target1copy->initImage(tcopy);

        pixelOperation(height, width, channels, target1, white1, dark1, target1copy);

        delete target1copy;


        //Copy and delete instantly after operation
        btrgb::Image* target2copy = new btrgb::Image("target2copy");
        cv::Mat tcopy2 = btrgb::Image::copyMatConvertDepth(target2->getMat(), CV_32F);
        target2copy->initImage(tcopy2);

        pixelOperation(height, width, channels, target2, white2, dark2, target2copy);

        delete target2copy;

    }

    // Store Results
    this->store_results(images);

    //Removes the white and dark images from the art object
    images->deleteImage("white1");
    images->deleteImage("white2");
    images->deleteImage("dark1");
    images->deleteImage("dark2");



    comms->send_progress(1, this->get_name());
    // Outputs TIFFs for each image group for after this step, temporary
    // images->outputImageAs(btrgb::TIFF, "art1", "art1_ff");
    // images->outputImageAs(btrgb::TIFF, "art2", "art2_ff");
}

/**
* Sets the w value for the two images for the flatfielding process
* @param pAvg: Average Pixel value of the second channel of the overall art image
* @param wAvg: Average Pixel value of the second channel of the overall white image
* @param yRef: y value from reference data
*/
void::FlatFieldor::wCalc(float pAvg, float wAvg, double yRef) {
    //w values are constants based on the y value and patch value averages
    this->w = ((yRef * (wAvg / pAvg)) / 100);
}

/**
* Updates the pixels based on the w calculation for both given images
* @param h: height of images
* @param wid: width of images
* @param c: channel count
* @param a1: art1 image
* @param a2: art2 image
* @param wh1: white1 image
* @param wh2: white2 image
* @param d1: dark1 image
* @param d2 : dark2 image
*/
void::FlatFieldor::pixelOperation(int h, int wid, int c, btrgb::Image* a, btrgb::Image* wh, btrgb::Image* d, btrgb::Image* ac) {
    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted based on the w for that group of images
    int currRow, currCol, ch;
    int stuckPixelCounter = 0;
    int uncorrectedCounter = 0;

    double wPix, dPix, aPix, newPixel;
    for (currRow = 0; currRow < h; currRow++) {
        for (currCol = 0; currCol < wid; currCol++) {
            for (ch = 0; ch < c; ch++) {

                //Get pixel from all three images
                wPix = wh->getPixel(currRow, currCol, ch);
                dPix = d->getPixel(currRow, currCol, ch);
                aPix = ac->getPixel(currRow, currCol, ch);

                //If pixel in white and dark targets are equal pixel is stuck / dead
                if (double(wPix == dPix)) {

                    stuckPixelCounter++;

                    //Radius to perform dead pixel correction
                    int radius = 2;

                    //Hold the final average value
                    double artPixelTotalValue = 0.0;
                    double whitePixeTotallValue = 0.0;
                    double darkPixeTotallValue = 0.0;

                    //Total number of neighboring pixels looked at
                    int pixelCount = 0;

                    //make sure the selection area doesn't go over the edge
                    int left = (currCol - radius < 0 ? 0 : currCol - radius);
                    int right = (currCol + radius >= wid ? wid - 1 : currCol + radius);
                    int top = (currRow - radius < 0 ? 0 : currRow - radius);
                    int bot = (currRow + radius > h ? h : currRow + radius);

                    for (auto xIndex : boost::irange(left, right)) {
                        for (auto yIndex : boost::irange(top, bot)) {

                            //Grab the values
                            double artPixel = ac->getPixel(yIndex, xIndex, ch);
                            double whitePixel = wh->getPixel(yIndex, xIndex, ch);
                            double darkPixel = d->getPixel(yIndex, xIndex, ch);

                            if (whitePixel != darkPixel) {
                                //Neighbor is not dead, so use to correct
                                pixelCount++;
                                artPixelTotalValue += artPixel;
                                whitePixeTotallValue += whitePixel;
                                darkPixeTotallValue += darkPixel;
                            }
                            else {
                                //Neighbor pixel is dead do nothing
                            }
                        }
                    }

                    //Average values
                    artPixelTotalValue = artPixelTotalValue / pixelCount;
                    whitePixeTotallValue = whitePixeTotallValue / pixelCount;
                    darkPixeTotallValue = darkPixeTotallValue / pixelCount;

                    //Perform flatfielding on these new values
                    newPixel = this->w * (double(artPixelTotalValue - darkPixeTotallValue) / double(whitePixeTotallValue - darkPixeTotallValue));

                    //Final sanity checks, ensure no invalid values get by
                    //NAN catch just set to 0;
                    if (newPixel != newPixel) {
                        uncorrectedCounter++;
                        newPixel = 0;
                    }
                    //INF catch just set to 0;
                    if (isinf(newPixel)) {
                        uncorrectedCounter++;
                        newPixel = 0;
                    }
                    //Done set pixel in artwork
                    a->setPixel(currRow, currCol, ch, newPixel);
                }

                //Normal pixels flatfield as normal
                else {
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));
                    a->setPixel(currRow, currCol, ch, newPixel);
                }
            }
        }
    }
    int corrected = stuckPixelCounter - uncorrectedCounter;
    std::cout << "Stuck/Dead Pixels Detected - " << stuckPixelCounter / 3 << "\n";
    std::cout << "Stuck/Dead Pixels Corrected - " << corrected / 3 << "\n";
    std::cout << "Stuck/Dead Pixels Uncorrected - " << uncorrectedCounter / 3 << "\n";
}

void FlatFieldor::store_results(btrgb::ArtObject* images) {
    CalibrationResults* results_obj = images->get_results_obj(btrgb::ResultType::GENERAL);
    RefData* reference = images->get_refrence_data();
    ColorTarget target = images->get_target(TARGET(1), btrgb::TargetType::GENERAL_TARGET);

    //Col and Row of the white patch on the target
    int whiteRow = target.get_white_row();
    int whiteCol = target.get_white_col();
    //Collecting the y Value from the reference data
    double y = reference->get_y(whiteRow, whiteCol);

    // Y whit patch meas
    results_obj->store_double(GI_Y, y);
    // W Value
    results_obj->store_double(GI_W, (double)this->w);

}
