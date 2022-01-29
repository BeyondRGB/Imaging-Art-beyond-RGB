#include "../header/FlatFieldor.h"

void FlatFieldor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    btrgb::image* art1;
    btrgb::image* art2;
    btrgb::image* white1;
    btrgb::image* white2;
    btrgb::image* dark1;
    btrgb::image* dark2;
    RefData* reference;

    std::cout<<"Variable Declaration"<<std::endl;

    func("Flat Fielding");
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

    std::cout<<"ArtObject Pulls"<<std::endl;

    //Set up of the size of all the bitmaps for all the different images being looked at
    int height = art1->height();
    int width = art1->width();
    int channels = art1->channels();
    btrgb::pixel* abitmap1 = art1->bitmap();
    btrgb::pixel* abitmap2 = art2->bitmap();
    btrgb::pixel* wbitmap1 = white1->bitmap();
    btrgb::pixel* wbitmap2 = white2->bitmap();
    btrgb::pixel* dbitmap1 = dark1->bitmap();
    btrgb::pixel* dbitmap2 = dark2->bitmap();

    std::cout<<"BitMap Declaration"<<std::endl;

    //ONLY NEED CHANNEL 2, aka Green channel, to get averages, y is from calc
    //Provided from Art Obj
    //Determines how many pixels are compared,
    //size - 1 = how many rings around the center point to be compared for avg
    //Current default to 3 can be adjusted if needed
    int size = 3;
    //Other data from Art Obj for Target
    double topLeftX = images->getTargetInfo("tlx");
    double topLeftY = images->getTargetInfo("tly");
    double topRightX = images->getTargetInfo("trx");
    double topRightY = images->getTargetInfo("try");
    double botRightX = images->getTargetInfo("brx");
    double botRightY = images->getTargetInfo("bry");
    double botLeftX = images->getTargetInfo("blx");
    double botLeftY = images->getTargetInfo("bly");
    int targetRows = images->getTargetSize("row");
    int targetCols = images->getTargetSize("col");

    std::cout<<"Target Data Declaration"<<std::endl;
    std::cout<<"*****************************"<<std::endl;
    std::cout<<"TopLeftY "<<topLeftY<<std::endl;
    std::cout<<"TopLeftX "<<topLeftX<<std::endl;
    std::cout<<"TopRightY "<<topRightY<<std::endl;
    std::cout<<"TopRightX "<<topRightX<<std::endl;
    std::cout<<"BotLeftY "<<botLeftY<<std::endl;
    std::cout<<"BotLeftX "<<botLeftX<<std::endl;
    std::cout<<"BotRightY "<<botRightY<<std::endl;
    std::cout<<"BotRightX "<<botRightX<<std::endl;
    std::cout<<"*****************************"<<std::endl;

    //Col and Row of the white patch on the target
    int whiteRow = reference->get_white_patch_row();
    int whiteCol = reference->get_white_patch_col();

    std::cout<<"Pull from Reference Data"<<std::endl;
    std::cout<<"*****************************"<<std::endl;
    std::cout<<"WhiteRow "<<whiteRow<<std::endl;
    std::cout<<"WhiteCol "<<whiteCol<<std::endl;
    std::cout<<"*****************************"<<std::endl;

    //Need to double check and make sure this can be done between ints and doubles
    //Normalized location by width from base
    int topEdge = width * topLeftY;
    int botEdge = width * botRightY;
    int leftEdge = width * botLeftX;
    int rightEdge = width * topRightX;
    int tarHeight = botEdge - topEdge;
    int tarWidth = rightEdge - leftEdge;
    //wOffY and wOffX are the offset from the edges of the color target to the white patch center
    int wOffY = tarHeight * (whiteRow / targetRows);
    int wOffX = tarWidth * (whiteCol / targetCols);

    //X and Y of White patch location
    int patchX = leftEdge + wOffX;
    int patchY = topEdge + wOffY;

    std::cout<<"Center Coordinate Declaration"<<std::endl;
    std::cout<<"*****************************"<<std::endl;
    std::cout<<"Top "<<topEdge<<std::endl;
    std::cout<<"Bot "<<botEdge<<std::endl;
    std::cout<<"Left "<<leftEdge<<std::endl;
    std::cout<<"Right "<<rightEdge<<std::endl;
    std::cout<<"Patch X "<<patchX<<std::endl;
    std::cout<<"Patch Y "<<patchY<<std::endl;
    std::cout<<"*****************************"<<std::endl;

    int art1Total = 0;
    int white1Total = 0;
    int art2Total = 0;
    int white2Total = 0;
    int loops = 0;
    int startVal = (size * -1) + 1;
    int xOff, yOff, i, ix, iy;

    std::cout<<"For Loop Variable Declaration"<<std::endl;

    //Collecting values of pixels in the rings around center pixel in the white
    for (yOff = startVal; yOff < size; yOff++){
        for (xOff = startVal; xOff < size; xOff++){
            std::cout<<"PatchX "<<patchX<<std::endl;
            std::cout<<"PatchY "<<patchY<<std::endl;
            std::cout<<"PatchX offset "<<xOff<<std::endl;
            std::cout<<"PatchY offset "<<yOff<<std::endl;
            iy = (patchY + yOff) * width * channels;
            ix = (patchX + xOff) * channels;
            i = iy + ix + 1;
            art1Total += abitmap1[i];
            white1Total += wbitmap1[i];
            art2Total += abitmap2[i];
            white2Total += wbitmap2[i];
            loops++;
            std::cout<<"Completed Loops "<<loops<<std::endl;
        }
    }

    std::cout<<"For Loop 1 Complete"<<std::endl;

    //Need to make sure that this is the correct denominator
    double art1Avg = art1Total / (size * size);
    double white1Avg = white1Total / (size * size);
    double art2Avg = art2Total / (size * size);
    double white2Avg = white2Total / (size * size);

    double yVal = reference->get_y(whiteRow, whiteCol);
    double w1 = yVal * (white1Avg / art1Avg);
    double w2 = yVal * (white2Avg / art2Avg);

    std::cout<<"W value calculated"<<std::endl;

    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted
    //Old version of the For Loop, may need to be changed
    int x, y, ch;
    int wPix, dPix, aPix;
    for (y = 0; y < height; y++) {
        iy = y * width * channels;
        for (x = 0; x < width; x++) {
            ix = x * channels;
            for (ch = 0; ch < channels; ch++) {
                i = iy + ix + ch;
                // i is the index for the bitmap
                wPix = wbitmap1[i];
                dPix = dbitmap1[i];
                aPix = abitmap1[i];
                //Need to overwrite previous image pixel in the Art Object
                abitmap1[i] = w1 * ((aPix - dPix) / (wPix - dPix));
                //Repeat for image 2
                wPix = wbitmap2[i];
                dPix = dbitmap2[i];
                aPix = abitmap2[i];
                abitmap2[i] = w2 * ((aPix - dPix) / (wPix - dPix));
            }
        }
    }

    std::cout<<"Flat Fielding Done"<<std::endl;

    sleep_for(seconds(1));
    //Need to add a call to turn into a TIFF
}
