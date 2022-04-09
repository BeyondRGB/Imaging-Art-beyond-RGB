#include "../header/FlatFieldor.h"
#include <iostream>
void FlatFieldor::execute(CommunicationObj *comms, btrgb::ArtObject *images)
{
    btrgb::Image *art1;
    btrgb::Image *art2;
    btrgb::Image *white1;
    btrgb::Image *white2;
    btrgb::Image *dark1;
    btrgb::Image *dark2;

    RefData *reference;

    comms->send_info("", "Flat Fielding");
    comms->send_progress(0, "Flat Fielding");

    btrgb::Image* art1copy = new btrgb::Image("art1copy");


    // Pull the images needed out of the Art Object
    try
    {
        art1 = images->getImage("art1");
        cv::Mat copy = btrgb::Image::copyMatConvertDepth(art1->getMat(), CV_32F);
        art1copy->initImage(copy);



        white1 = images->getImage("white1");
        dark1 = images->getImage("dark1");
        art2 = images->getImage("art2");
        white2 = images->getImage("white2");
        dark2 = images->getImage("dark2");
        reference = images->get_refrence_data();


        images->outputImageAs(btrgb::TIFF, "art1", "artbefore");

        images->outputImageAs(btrgb::TIFF, "white1", "white");
        images->outputImageAs(btrgb::TIFF, "dark1", "dark");

        images->outputImageAs(btrgb::TIFF, "white2", "white2");
        images->outputImageAs(btrgb::TIFF, "dark2", "dark2");


        /*
        cv::FileStorage file("art1.yml", cv::FileStorage::WRITE);
        cv::Mat im1 = art1->getMat();
        file << "matName" << im1;

        cv::FileStorage file1("white1.yml", cv::FileStorage::WRITE);
        cv::Mat im2 = white1->getMat();
        file1 << "matName" << im2;

        cv::FileStorage file2("dark1.yml", cv::FileStorage::WRITE);
        cv::Mat im3 = dark1->getMat();
        file2 << "matName" << im3;
        */




    }
    catch (const btrgb::ArtObj_ImageDoesNotExist &e)
    {
        comms->send_error("Error: Flatfielding called out of order. Missing at least 1 image assignment.", "FlatFieldor");
        return;
    }

    // Set up variables for the overall size of all the images, they are all the same size
    int height = art1->height();
    int width = art1->width();
    int channels = art1->channels();

    //Col and Row of the white patch on the target
    int whiteRow = reference->get_white_patch_row();
    int whiteCol = reference->get_white_patch_col();

    //Collecting the y Value from the reference data
    double yVal = reference->get_y(whiteRow, whiteCol);

    //Getting average patch values for white and art images channel two
    float patAvg = images->get_target("art1").get_patch_avg(whiteRow, whiteCol, 1);
    float whiteAvg = images->get_target("white1").get_patch_avg(whiteRow, whiteCol, 1);

    //Calculate w value and complete the pixel operation with set w value
    wCalc(patAvg, whiteAvg, yVal);
    pixelOperation(height, width, channels, art1, art2, white1, white2, dark1, dark2, art1copy);

    // Store Results
    this->store_results(images);

    //Removes the white and dark images from the art object
    images->deleteImage("white1");
    images->deleteImage("white2");
    images->deleteImage("dark1");
    images->deleteImage("dark2");
    delete art1copy;
    std::cout << "flat Time";

    comms->send_progress(1, "Flat Fielding");
    // Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "art1_ff");
    images->outputImageAs(btrgb::TIFF, "art2", "art2_ff");
}

/**
* Sets the w value for the two images for the flatfielding process
* @param pAvg: Average Pixel value of the second channel of the overall art image
* @param wAvg: Average Pixel value of the second channel of the overall white image
* @param yRef: y value from reference data
*/
void::FlatFieldor::wCalc(float pAvg, float wAvg, double yRef){
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
void::FlatFieldor::pixelOperation(int h, int wid, int c, btrgb::Image* a1, btrgb::Image* a2, btrgb::Image* wh1, btrgb::Image* wh2, btrgb::Image* d1, btrgb::Image* d2, btrgb::Image* a1c){
    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted based on the w for that group of images
    int currRow, currCol, ch;
    int stuckPixelCounter = 0;
    int uncorrectedCounter = 0;
    double wPix, dPix, aPix, newPixel;
    for (currRow = 0; currRow < h; currRow++) {
        for (currCol = 0; currCol < wid; currCol++) {
            for (ch = 0; ch < c; ch++) {



                //Get pixels
                wPix = wh1->getPixel(currRow, currCol, ch);
                dPix = d1->getPixel(currRow, currCol, ch);
                aPix = a1->getPixel(currRow, currCol, ch);

       
                //If pixels are equal across image set 1 means the pixel is stuck/dead.
                if (double(wPix == dPix) || double(aPix == dPix)) {

                    //Stuck pixel detected
                    stuckPixelCounter++;

                    
                    //std::cout << "Dead \n";
                    //std::cout << wPix << "\n";
                    //std::cout << dPix << "\n";
                    //std::cout << aPix << "\n";

                    //Get different pixels todo make this not suck.
                    //Large blobs are dead so we must look at a bunch

                    // X = stuck
                    /*
                           EAF
                           CXD
                           GBH


                           R
                           O
                           W

                           Column
                    */


                    //Shitty edge detection implemented!

                    double wPixA, dPixA, aPixA;
                    double wPixB, dPixB, aPixB;
                    double wPixC, dPixC, aPixC;
                    double wPixD, dPixD, aPixD;
                    double wPixE, dPixE, aPixE;
                    double wPixF, dPixF, aPixF;
                    double wPixG, dPixG, aPixG;
                    double wPixH, dPixH, aPixH;

                    //A
                    if (currRow != 0) {
                        wPixA = wh1->getPixel(currRow - 1, currCol, ch);
                        dPixA = d1->getPixel(currRow - 1, currCol, ch);
                        aPixA = a1c->getPixel(currRow - 1, currCol, ch);
                    }
                    else {
                        wPixA = wPix;
                        dPixA = dPix;
                        aPixA = aPix;
                    }

                    //B
                    if (currRow != h) {
                        wPixB = wh1->getPixel(currRow + 1, currCol, ch);
                        dPixB = d1->getPixel(currRow + 1, currCol, ch);
                        aPixB = a1c->getPixel(currRow + 1, currCol, ch);
                    }
                    else {
                        wPixB = wPix;
                        dPixB = dPix;
                        aPixB = aPix;
                    }

                    //C
                    if (currCol != 0) {
                        wPixC = wh1->getPixel(currRow, currCol - 1, ch);
                        dPixC = d1->getPixel(currRow, currCol - 1, ch);
                        aPixC = a1c->getPixel(currRow, currCol - 1, ch);
                    }
                    else {
                        wPixC = wPix;
                        dPixC = dPix;
                        aPixC = aPix;
                    }

                    //D
                    if (currCol != wid) {
                        wPixD = wh1->getPixel(currRow, currCol + 1, ch);
                        dPixD = d1->getPixel(currRow, currCol + 1, ch);
                        aPixD = a1c->getPixel(currRow, currCol + 1, ch);
                    }
                    else {
                        wPixD = wPix;
                        dPixD = dPix;
                        aPixD = aPix;
                    }

                    //E
                    if (currRow != 0 && currCol != 0) {
                        wPixE = wh1->getPixel(currRow - 1, currCol - 1, ch);
                        dPixE = d1->getPixel(currRow - 1, currCol - 1, ch);
                        aPixE = a1c->getPixel(currRow - 1, currCol - 1, ch);
                    }
                    else {
                        wPixE = wPix;
                        dPixE = dPix;
                        aPixE = aPix;
                    }

                    //F
                    if (currRow != 0 && currCol != h) {
                        wPixF = wh1->getPixel(currRow - 1, currCol + 1, ch);
                        dPixF = d1->getPixel(currRow - 1, currCol + 1, ch);
                        aPixF = a1c->getPixel(currRow - 1, currCol + 1, ch);
                    }
                    else {
                        wPixF = wPix;
                        dPixF = dPix;
                        aPixF = aPix;
                    }

                    //G
                    if (currRow != h && currCol != 0) {
                        wPixG = wh1->getPixel(currRow + 1, currCol - 1, ch);
                        dPixG = d1->getPixel(currRow + 1, currCol - 1, ch);
                        aPixG = a1c->getPixel(currRow + 1, currCol - 1, ch);
                    }
                    else {
                        wPixG = wPix;
                        dPixG = dPix;
                        aPixG = aPix;
                    }

                    //H
                    if (currRow != h && currCol != wid) {
                        wPixH = wh1->getPixel(currRow + 1, currCol + 1, ch);
                        dPixH = d1->getPixel(currRow + 1, currCol + 1, ch);
                        aPixH = a1c->getPixel(currRow + 1, currCol + 1, ch);
                    }
                    else {
                        wPixH = wPix;
                        dPixH = dPix;
                        aPixH = aPix;
                    }

                    //Blend the dead pixels into their neighbors
                    wPix = double((wPixA + wPixB + wPixC + wPixD + wPixE + wPixF + wPixG + wPixH) / 8);
                    dPix = double((dPixA + dPixB + dPixC + dPixD + dPixE + dPixF + dPixG + dPixH) / 8);
                    aPix = double((aPixA + aPixB + aPixC + aPixD + aPixE + aPixF + aPixG + aPixH) / 8);

                    //std::cout << "Corrected \n";
                    //std::cout << wPix << "\n";
                    //std::cout << dPix << "\n";
                    //std::cout << aPix << "\n";

                    //std::cout << "Art blend targets \n";

                    //std::cout << aPixA << "\n";
                    //std::cout << aPixB << "\n";
                    //std::cout << aPixC << "\n";
                    //std::cout << aPixD << "\n";
                    //std::cout << aPixE << "\n";
                    //std::cout << aPixF << "\n";
                    //std::cout << aPixG << "\n";
                    //std::cout << aPixH << "\n";


                    //getchar();
               
                    //Calibrate
                    

                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));



                    //NAN catch just set to 0;
                    if (newPixel != newPixel) {
                        uncorrectedCounter++;
                        std::cout << "NAN" << "\n";
                        std::cout << wPix << "\n";
                        std::cout << dPix << "\n";
                        std::cout << aPix << "\n";
                        newPixel = 0;
                    }


                    //INF catch just set to 0;
                    if (isinf(newPixel)) {
                        uncorrectedCounter++;
                        std::cout << "INF" << "\n";
                        std::cout << wPix << "\n";
                        std::cout << dPix << "\n";
                        std::cout << aPix << "\n";
                        newPixel = 0;
                    }

                    //Done set pixel
                    a1->setPixel(currRow, currCol, ch, newPixel);


                }
                //Normal pixels continue with normal correction.
                else {
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));
                    a1->setPixel(currRow, currCol, ch, newPixel);
                  
                }

                /*
                // Repeat for image set 2
                wPix = wh2->getPixel(currRow, currCol, ch);
                dPix = d2->getPixel(currRow, currCol, ch);
                aPix = a2->getPixel(currRow, currCol, ch);
                //If pixels are equal across image set 2 means the pixel is stuck/dead.
                if (double(wPix - dPix) == 0) {

              
                    //Shitty edge detection implemented!
                    double wPixA, dPixA, aPixA;
                    double wPixB, dPixB, aPixB;
                    double wPixC, dPixC, aPixC;
                    double wPixD, dPixD, aPixD;
                    double wPixE, dPixE, aPixE;
                    double wPixF, dPixF, aPixF;
                    double wPixG, dPixG, aPixG;
                    double wPixH, dPixH, aPixH;



                    if (currRow != 0) {
                        wPixA = wh2->getPixel(currRow - 1, currCol, ch);
                        dPixA = d2->getPixel(currRow - 1, currCol, ch);
                        aPixA = a2->getPixel(currRow - 1, currCol, ch);
                    }
                    else {
                        wPixA = wh2->getPixel(currRow, currCol, ch);
                        dPixA = d2->getPixel(currRow, currCol, ch);
                        aPixA = a2->getPixel(currRow, currCol, ch);
                    }

                    if (currRow != h) {
                        wPixB = wh2->getPixel(currRow + 1, currCol, ch);
                        dPixB = d2->getPixel(currRow + 1, currCol, ch);
                        aPixB = a2->getPixel(currRow + 1, currCol, ch);
                    }
                    else {
                        wPixB = wh2->getPixel(currRow, currCol, ch);
                        dPixB = d2->getPixel(currRow, currCol, ch);
                        aPixB = a2->getPixel(currRow, currCol, ch);
                    }

                    if (currCol != 0) {
                        wPixC = wh2->getPixel(currRow, currCol - 1, ch);
                        dPixC = d2->getPixel(currRow, currCol - 1, ch);
                        aPixC = a2->getPixel(currRow, currCol - 1, ch);
                    }
                    else {
                        wPixC = wh2->getPixel(currRow, currCol, ch);
                        dPixC = d2->getPixel(currRow, currCol, ch);
                        aPixC = a2->getPixel(currRow, currCol, ch);
                    }

                    if (currCol != wid) {
                        wPixD = wh2->getPixel(currRow, currCol + 1, ch);
                        dPixD = d2->getPixel(currRow, currCol + 1, ch);
                        aPixD = a2->getPixel(currRow, currCol + 1, ch);
                    }
                    else {
                        wPixD = wh2->getPixel(currRow, currCol, ch);
                        dPixD = d2->getPixel(currRow, currCol, ch);
                        aPixD = a2->getPixel(currRow, currCol, ch);
                    }

                    if (currCol != 0 && currRow != 0) {
                        wPixE = wh2->getPixel(currRow - 1, currCol - 1, ch);
                        dPixE = d2->getPixel(currRow - 1, currCol - 1, ch);
                        aPixE = a2->getPixel(currRow - 1, currCol - 1, ch);
                    }
                    else {
                        wPixE = wh2->getPixel(currRow, currCol, ch);
                        dPixE = d2->getPixel(currRow, currCol, ch);
                        aPixE = a2->getPixel(currRow, currCol, ch);
                    }


                    //F
                    if (currRow != 0 && currCol != h) {
                        wPixF = wh2->getPixel(currRow - 1, currCol + 1, ch);
                        dPixF = d2->getPixel(currRow - 1, currCol + 1, ch);
                        aPixF = a2->getPixel(currRow - 1, currCol + 1, ch);
                    }
                    else {
                        wPixF = wh2->getPixel(currRow, currCol, ch);
                        dPixF = d2->getPixel(currRow, currCol, ch);
                        aPixF = a2->getPixel(currRow, currCol, ch);
                    }

                    //G
                    if (currRow != h && currCol != 0) {
                        wPixG = wh2->getPixel(currRow + 1, currCol - 1, ch);
                        dPixG = d2->getPixel(currRow + 1, currCol - 1, ch);
                        aPixG = a2->getPixel(currRow + 1, currCol - 1, ch);
                    }
                    else {
                        wPixG = wh2->getPixel(currRow, currCol, ch);
                        dPixG = d2->getPixel(currRow, currCol, ch);
                        aPixG = a2->getPixel(currRow, currCol, ch);
                    }

                    //H
                    if (currRow != h && currCol != wid) {
                        wPixH = wh2->getPixel(currRow + 1, currCol + 1, ch);
                        dPixH = d2->getPixel(currRow + 1, currCol + 1, ch);
                        aPixH = a2->getPixel(currRow + 1, currCol + 1, ch);
                    }
                    else {
                        wPixH = wh2->getPixel(currRow, currCol, ch);
                        dPixH = d2->getPixel(currRow, currCol, ch);
                        aPixH = a2->getPixel(currRow, currCol, ch);
                    }


                    wPix = double((wPixA + wPixB + wPixC + wPixD + wPixE + wPixF + wPixG + wPixH) / 8);
                    dPix = double((dPixA + dPixB + dPixC + dPixD + dPixE + dPixF + dPixG + dPixH) / 8);
                    aPix = double((aPixA + aPixB + aPixC + aPixD + aPixE + aPixF + aPixG + aPixH) / 8);

                    //Calibrate
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));

                    //getchar();
                    a2->setPixel(currRow, currCol, ch, newPixel);

                }
                //Normal pixels continue with normal correction.
                else {
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));
                    a2->setPixel(currRow, currCol, ch, newPixel);

                }
                */
            }
        }
    }

    std::cout << stuckPixelCounter << "\n";
}

void FlatFieldor::store_results(btrgb::ArtObject* images){
    CalibrationResults *results_obj = images->get_results_obj(btrgb::ResultType::GENERAL);

    RefData *reference = images->get_refrence_data();
    //Col and Row of the white patch on the target
    int whiteRow = reference->get_white_patch_row();
    int whiteCol = reference->get_white_patch_col();
    //Collecting the y Value from the reference data
    double y = reference->get_y(whiteRow, whiteCol);

    // Y whit patch meas
    results_obj->store_double(GI_Y, y);
    // W Value
    results_obj->store_double(GI_W, (double)this->w);

}
