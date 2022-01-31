#include "../header/FlatFieldor.h"

void FlatFieldor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    btrgb::image* art1;
    btrgb::image* art2;
    btrgb::image* white1;
    btrgb::image* white2;
    btrgb::image* dark1;
    btrgb::image* dark2;
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

    //size - 1 = how many rings around the center point to be compared for avg
    //Current default to 3 can be adjusted if needed
    int size = 3;

    //Collect Normalized Target Information From the Art Object
    double topTarget = images->getTargetInfo("top");
    double botTarget = images->getTargetInfo("bot");
    double leftTarget = images->getTargetInfo("left");
    double rightTarget = images->getTargetInfo("right");
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

    //Setting values for the For Loop going over one channel, channel 2
    int art1Total = 0;
    int white1Total = 0;
    int art2Total = 0;
    int white2Total = 0;
    int loops = 0;
    int startVal = (size * -1) + 1;
    int xOff, yOff, currRow, currCol;

    //Collecting values of pixels in the rings around center pixel in the white patch
    for (yOff = startVal; yOff < size; yOff++){
        for (xOff = startVal; xOff < size; xOff++){
            currRow = (patchY + yOff);
            currCol = (patchX + xOff);
            art1Total += art1->getPixel(currRow, currCol, 1);
            white1Total += white1->getPixel(currRow, currCol, 1);
            art2Total += art2->getPixel(currRow, currCol, 1);
            white2Total += white2->getPixel(currRow, currCol, 1);
            loops++;
        }
    }

    //Calculate average based on the counts from the for loop
    double art1Avg = art1Total / (loops);
    double white1Avg = white1Total / (loops);
    double art2Avg = art2Total / (loops);
    double white2Avg = white2Total / (loops);

    //Y value is calculated in ref_data
    //w values are constants based on the y value and patch value averages
    double yVal = reference->get_y(whiteRow, whiteCol);
    //double w1 = yVal * (white1Avg / art1Avg);
    double w1 = ((yVal * (white1Avg / art1Avg)) / 100) * 0xFFFF;
    //double w2 = yVal * (white2Avg / art2Avg);
    double w2 = ((yVal * (white2Avg / art2Avg)) / 100) * 0xFFFF;

    //This was for testing the output of the y and w values
    /*std::cout<<"****************************"<<std::endl;
    std::cout<<"yVal "<<yVal<<std::endl;
    std::cout<<"W value 1 "<<w1<<std::endl;
    std::cout<<"W value 2 "<<w2<<std::endl;
    std::cout<<"****************************"<<std::endl;*/

    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted based on the w for that group of images
    int ch;
    int wPix, dPix, aPix, newPixel;
    for (currRow = 0; currRow < height; currRow++) {
        for (currCol = 0; currCol < width; currCol++) {
            for (ch = 0; ch < channels; ch++) {
                wPix = white1->getPixel(currRow, currCol, ch);
                dPix = dark1->getPixel(currRow, currCol, ch);
                aPix = art1->getPixel(currRow, currCol, ch);
                //Need to overwrite previous image pixel in the Art Object
                newPixel = w1 * (double(aPix - dPix) / double(wPix - dPix));
                art1->setPixel(currRow, currCol, ch, newPixel);
                //Repeat for image 2
                wPix = white2->getPixel(currRow, currCol, ch);
                dPix = dark2->getPixel(currRow, currCol, ch);
                aPix = art2->getPixel(currRow, currCol, ch);;
                newPixel = w2 * (double(aPix - dPix) / double(wPix - dPix));
                art2->setPixel(currRow, currCol, ch, newPixel);
            }
        }
    }
    
    //Outputs TIFFs for each image group for after this step, temporary
    images->outputImageAs(btrgb::TIFF, "art1", "FFOut1");
    images->outputImageAs(btrgb::TIFF, "art2", "FFOut2");
}
