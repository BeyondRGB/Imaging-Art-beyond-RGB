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
    wCalc(startVal, size, patchX, patchY, yVal, art1, art2, white1, white2);
    pixelOperation(height, width, channels, art1, art2, white1, white2, dark1, dark2);

    //Removes the white and dark images from the art object
    images->deleteImage("white1");
    images->deleteImage("white2");
    images->deleteImage("dark1");
    images->deleteImage("dark2");
}
//Takes in the size as rings, the corrdinates of the white patech, the y value, and the images needed for this function
//Sets the w value for the two images for the flatfielding process
void::FlatFieldor::wCalc(int base, int rings, int patX, int patY, double yRef, btrgb::image* a1, btrgb::image* a2, btrgb::image* wh1, btrgb::image* wh2){
    //Setting values for the For Loop going over one channel, channel 2
    int art1Total = 0;
    int white1Total = 0;
    int art2Total = 0;
    int white2Total = 0;
    int loops = 0;
    int xOff, yOff, currRow, currCol;

    //Collecting values of pixels in the rings around center pixel in the white patch
    for (yOff = base; yOff < rings; yOff++){
        for (xOff = base; xOff < rings; xOff++){
            currRow = (patY + yOff);
            currCol = (patX + xOff);
            art1Total += a1->getPixel(currRow, currCol, 1);
            white1Total += wh1->getPixel(currRow, currCol, 1);
            art2Total += a2->getPixel(currRow, currCol, 1);
            white2Total += wh2->getPixel(currRow, currCol, 1);
            loops++;
        }
    }
    //Calculate average based on the counts from the for loop
    double art1Avg = art1Total / (loops);
    double white1Avg = white1Total / (loops);
    double art2Avg = art2Total / (loops);
    double white2Avg = white2Total / (loops);

    //w values are constants based on the y value and patch value averages
    w1 = ((yRef * (white1Avg / art1Avg)) / 100);
    w2 = ((yRef * (white2Avg / art2Avg)) / 100);
}

//Takes in the height, width, and channel count of the images in general, and takes in all the images that were provided by the user
//Updates the pixels based on the w calculation for both the images
void::FlatFieldor::pixelOperation(int h, int w, int c, btrgb::image* a1, btrgb::image* a2, btrgb::image* wh1, btrgb::image* wh2, btrgb::image* d1, btrgb::image* d2){
    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted based on the w for that group of images
    int currRow, currCol, ch;
    int wPix, dPix, aPix, newPixel;
    for (currRow = 0; currRow < h; currRow++) {
        for (currCol = 0; currCol < w; currCol++) {
            for (ch = 0; ch < c; ch++) {
                wPix = wh1->getPixel(currRow, currCol, ch);
                dPix = d1->getPixel(currRow, currCol, ch);
                aPix = a1->getPixel(currRow, currCol, ch);
                //Need to overwrite previous image pixel in the Art Object
                newPixel = w1 * (double(aPix - dPix) / double(wPix - dPix));
                a1->setPixel(currRow, currCol, ch, newPixel);
                //Repeat for image 2
                wPix = wh2->getPixel(currRow, currCol, ch);
                dPix = d2->getPixel(currRow, currCol, ch);
                aPix = a2->getPixel(currRow, currCol, ch);
                newPixel = w2 * (double(aPix - dPix) / double(wPix - dPix));
                a2->setPixel(currRow, currCol, ch, newPixel);
            }
        }
    }
}
