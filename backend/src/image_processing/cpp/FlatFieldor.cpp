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


    comms->send_info("", this->get_name());
    comms->send_progress(0, this->get_name());

    //Unmodified copies used for dead pixel correction in flat fielding.
    btrgb::Image* art1copy = new btrgb::Image("art1copy");
    btrgb::Image* art2copy = new btrgb::Image("art2copy");


    // Pull the images needed out of the Art Object
    try
    {
        art1 = images->getImage("art1");
        cv::Mat copy = btrgb::Image::copyMatConvertDepth(art1->getMat(), CV_32F);
        art1copy->initImage(copy);

        art2 = images->getImage("art2");
        cv::Mat copy2 = btrgb::Image::copyMatConvertDepth(art1->getMat(), CV_32F);
        art2copy->initImage(copy2);


        white1 = images->getImage("white1");
        dark1 = images->getImage("dark1");
        white2 = images->getImage("white2");
        dark2 = images->getImage("dark2");
        reference = images->get_refrence_data();

        //Uncomment to write pixel data to file.
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
        throw ImgProcessingComponent::error(e.what(), this->get_name());
    }

    // Set up variables for the overall size of all the images, they are all the same size
    int height = art1->height();
    int width = art1->width();
    int channels = art1->channels();

    ColorTarget target = images->get_target("art1", btrgb::TargetType::GENERAL_TARGET);


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
    pixelOperation(height, width, channels, art1, art2, white1, white2, dark1, dark2, art1copy, art2copy);

    // Store Results
    this->store_results(images);

    //Removes the white and dark images from the art object
    images->deleteImage("white1");
    images->deleteImage("white2");
    images->deleteImage("dark1");
    images->deleteImage("dark2");
    delete art1copy;
    delete art2copy;

    comms->send_progress(1, this->get_name());
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
void::FlatFieldor::pixelOperation(int h, int wid, int c, btrgb::Image* a1, btrgb::Image* a2, btrgb::Image* wh1, btrgb::Image* wh2, btrgb::Image* d1, btrgb::Image* d2, btrgb::Image* a1c, btrgb::Image* a2c){
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
                wPix = wh1->getPixel(currRow, currCol, ch);
                dPix = d1->getPixel(currRow, currCol, ch);
                aPix = a1->getPixel(currRow, currCol, ch);

                //If pixel in white and dark targets are equal pixel is stuck / dead
                if (double(wPix == dPix)) {

                    //Stuck pixel detected
                    stuckPixelCounter++;

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

                    //Flat field the new pixel value
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));

                    //Some larger groups may have a center pixel slip by. Following checks forces them to 0 and logs it.
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
                    a1->setPixel(currRow, currCol, ch, newPixel);

                }
                //Normal pixels continue with normal correction.
                else {
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));
                    a1->setPixel(currRow, currCol, ch, newPixel);
                }

                //Repeat for image set 2

                //Get pixel from all three images
                wPix = wh2->getPixel(currRow, currCol, ch);
                dPix = d2->getPixel(currRow, currCol, ch);
                aPix = a2->getPixel(currRow, currCol, ch);

                //If pixel in white and dark targets are equal pixel is stuck / dead
                if (double(wPix == dPix)) {
                    //Stuck pixel detected
                    stuckPixelCounter++;
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
                        wPixA = wh2->getPixel(currRow - 1, currCol, ch);
                        dPixA = d2->getPixel(currRow - 1, currCol, ch);
                        aPixA = a2c->getPixel(currRow - 1, currCol, ch);
                    }
                    else {
                        wPixA = wPix;
                        dPixA = dPix;
                        aPixA = aPix;
                    }
                    //B
                    if (currRow != h) {
                        wPixB = wh2->getPixel(currRow + 1, currCol, ch);
                        dPixB = d2->getPixel(currRow + 1, currCol, ch);
                        aPixB = a2c->getPixel(currRow + 1, currCol, ch);
                    }
                    else {
                        wPixB = wPix;
                        dPixB = dPix;
                        aPixB = aPix;
                    }
                    //C
                    if (currCol != 0) {
                        wPixC = wh2->getPixel(currRow, currCol - 1, ch);
                        dPixC = d2->getPixel(currRow, currCol - 1, ch);
                        aPixC = a2c->getPixel(currRow, currCol - 1, ch);
                    }
                    else {
                        wPixC = wPix;
                        dPixC = dPix;
                        aPixC = aPix;
                    }
                    //D
                    if (currCol != wid) {
                        wPixD = wh2->getPixel(currRow, currCol + 1, ch);
                        dPixD = d2->getPixel(currRow, currCol + 1, ch);
                        aPixD = a2c->getPixel(currRow, currCol + 1, ch);
                    }
                    else {
                        wPixD = wPix;
                        dPixD = dPix;
                        aPixD = aPix;
                    }
                    //E
                    if (currRow != 0 && currCol != 0) {
                        wPixE = wh2->getPixel(currRow - 1, currCol - 1, ch);
                        dPixE = d2->getPixel(currRow - 1, currCol - 1, ch);
                        aPixE = a2c->getPixel(currRow - 1, currCol - 1, ch);
                    }
                    else {
                        wPixE = wPix;
                        dPixE = dPix;
                        aPixE = aPix;
                    }
                    //F
                    if (currRow != 0 && currCol != h) {
                        wPixF = wh2->getPixel(currRow - 1, currCol + 1, ch);
                        dPixF = d2->getPixel(currRow - 1, currCol + 1, ch);
                        aPixF = a2c->getPixel(currRow - 1, currCol + 1, ch);
                    }
                    else {
                        wPixF = wPix;
                        dPixF = dPix;
                        aPixF = aPix;
                    }
                    //G
                    if (currRow != h && currCol != 0) {
                        wPixG = wh2->getPixel(currRow + 1, currCol - 1, ch);
                        dPixG = d2->getPixel(currRow + 1, currCol - 1, ch);
                        aPixG = a2c->getPixel(currRow + 1, currCol - 1, ch);
                    }
                    else {
                        wPixG = wPix;
                        dPixG = dPix;
                        aPixG = aPix;
                    }
                    //H
                    if (currRow != h && currCol != wid) {
                        wPixH = wh2->getPixel(currRow + 1, currCol + 1, ch);
                        dPixH = d2->getPixel(currRow + 1, currCol + 1, ch);
                        aPixH = a2c->getPixel(currRow + 1, currCol + 1, ch);
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

                    //Flat field the new pixel value
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));

                    //Some larger groups may have a center pixel slip by. Following checks forces them to 0 and logs it.
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
                    a2->setPixel(currRow, currCol, ch, newPixel);

                }
                //Normal pixels continue with normal correction.
                else {
                    newPixel = this->w * (double(aPix - dPix) / double(wPix - dPix));
                    a2->setPixel(currRow, currCol, ch, newPixel);
                }
            }
        }
    }
    int corrected = stuckPixelCounter - uncorrectedCounter;
    std::cout << "Stuck/Dead Pixels Detected - " << stuckPixelCounter / 6<< "\n";
    std::cout << "Stuck/Dead Pixels Corrected - " << corrected / 6 << "\n";
    std::cout << "Stuck/Dead Pixels Uncorrected - " << uncorrectedCounter / 6<< "\n";
}

void FlatFieldor::store_results(btrgb::ArtObject* images) {
    CalibrationResults* results_obj = images->get_results_obj(btrgb::ResultType::GENERAL);

    RefData* reference = images->get_refrence_data();
    ColorTarget target = images->get_target("art1", btrgb::TargetType::GENERAL_TARGET);
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