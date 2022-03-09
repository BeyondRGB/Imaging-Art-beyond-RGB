#include "../header/FlatFieldor.h"

//Runs the overall function for correcting the white and dark coloring of the image
void FlatFieldor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    btrgb::Image* art1;
    btrgb::Image* art2;
    btrgb::Image* white1;
    btrgb::Image* white2;
    btrgb::Image* dark1;
    btrgb::Image* dark2;
    RefData* reference;

    func("Flat Fielding");

    //Pull the images needed out of the Art Object
    try {
        art1 = images->getImage("art1");
        white1 = images->getImage("white1");
        dark1 = images->getImage("dark1");
        art2 = images->getImage("art2");
        white2 = images->getImage("white2");
        dark2 = images->getImage("dark2");
        reference = images->get_refrence_data();
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        func("Error: Flatfielding called out of order. Missing at least 1 image assignment.");
        return;
    }

    //Set up variables for the overall size of all the images, they are all the same size
    int height = art1->height();
    int width = art1->width();
    int channels = art1->channels();

    //Collect Normalized Target Information From the Art Object
    float topTarget = images->getTargetInfo("top");
    float botTarget = images->getTargetInfo("bot");
    float leftTarget = images->getTargetInfo("left");
    float rightTarget = images->getTargetInfo("right");
    int targetRows = images->getTargetSize("row");
    int targetCols = images->getTargetSize("col");

    //Col and Row of the white patch on the target
    int whiteRow = reference->get_white_patch_row();
    int whiteCol = reference->get_white_patch_col();

    //Change Normalized values to actual pixel coordinates
    int topEdge = width * topTarget;
    int botEdge = width * botTarget;
    int leftEdge = width * leftTarget;
    int rightEdge = width * rightTarget;
    int tarHeight = botEdge - topEdge;
    int tarWidth = rightEdge - leftEdge;

    //wOffY and wOffX are the offset from the edges of the color target to the white patch center
    int rowHeight = (tarHeight / targetRows);
    int colWidth = (tarWidth / targetCols);
    int wOffY = whiteRow * rowHeight;
    int wOffX = whiteCol * colWidth;

    //X and Y of White patch location
    //Offset by half of a cells width and height to get the center
    int patchX = leftEdge + wOffX + (colWidth / 2);
    int patchY = topEdge + wOffY + (rowHeight / 2);

    //size - 1 = how many rings around the center point to be compared for avg
    //Current default to 3 can be adjusted if needed
    int size = 3;
    int startVal = (size * -1) + 1;

    //Collecting the y Value from the reference data
    double yVal = reference->get_y(whiteRow, whiteCol);
    float patAvg = images->get_target("art1").get_patch_avg(whiteRow, whiteCol, 1);
    float whiteAvg = images->get_target("white1").get_patch_avg(whiteRow, whiteCol, 1);
    wCalc(patAvg, whiteAvg, yVal);
    pixelOperation(height, width, channels, art1, art2, white1, white2, dark1, dark2);

    //Testing Image Outputs
    //images->outputImageAs(btrgb::TIFF, "art1", "FFOut1");
    //images->outputImageAs(btrgb::TIFF, "art2", "FFOut2");
    //Removes the white and dark images from the art object
    images->deleteImage("white1");
    images->deleteImage("white2");
    images->deleteImage("dark1");
    images->deleteImage("dark2");
}

/**
* Sets the w value for the two images for the flatfielding process
* @param pAvg: Average Pixel value of the second channel of the overall art image
* @param wAvg: Average Pixel value of the second channel of the overall white image
* @param yRef: y value from reference data
*/
void::FlatFieldor::wCalc(float pAvg, float wAvg, double yRef){
    //w values are constants based on the y value and patch value averages
    w = ((yRef * (wAvg / pAvg)) / 100);
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
void::FlatFieldor::pixelOperation(int h, int wid, int c, btrgb::Image* a1, btrgb::Image* a2, btrgb::Image* wh1, btrgb::Image* wh2, btrgb::Image* d1, btrgb::Image* d2){
    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted based on the w for that group of images
    int currRow, currCol, ch;
    float wPix, dPix, aPix, newPixel;
    for (currRow = 0; currRow < h; currRow++) {
        for (currCol = 0; currCol < wid; currCol++) {
            for (ch = 0; ch < c; ch++) {
                wPix = wh1->getPixel(currRow, currCol, ch);
                dPix = d1->getPixel(currRow, currCol, ch);
                aPix = a1->getPixel(currRow, currCol, ch);
                //Need to overwrite previous image pixel in the Art Object
                newPixel = w * (double(aPix - dPix) / double(wPix - dPix));
                a1->setPixel(currRow, currCol, ch, newPixel);
                //Repeat for image 2
                wPix = wh2->getPixel(currRow, currCol, ch);
                dPix = d2->getPixel(currRow, currCol, ch);
                aPix = a2->getPixel(currRow, currCol, ch);
                newPixel = w * (double(aPix - dPix) / double(wPix - dPix));
                a2->setPixel(currRow, currCol, ch, newPixel);
            }
        }
    }
}
