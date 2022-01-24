//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/FlatFeildor.h"

void FlatFeildor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Flat Fielding");
    //TO BE DEPREICATED
/*    int patches;
    float cell[35][patches];
    int highestIndex;
    float highestAvg = 0;
    for (int i = 0; i < patches; i++) {
        float tempAvg = 0;
        float tempTotal = 0;
        for (int j = 0; j < 35; j++) {
            tempTotal += cell[j][i];
        }
        tempAvg = tempTotal / 35;
        if (tempAvg > highestAvg) {
            highestIndex = i;
            highestAvg = tempAvg;
        }
    }
    */
    try {
        btrgb::image* art1 = images->getImage("art1");
        btrgb::image* white1 = images->getImage("white1");;
        btrgb::image* dark1 = images->getImage("dark1");;
        btrgb::image* art2 = images->getImage("art2");;
        btrgb::image* white2 = images->getImage("white2");;
        btrgb::image* dark2 = images->getImage("dark2");;
    }
    catch (const btrgb::ArtObj_ImageDoesNotExist& e) {
        func("Error: Flatfielding called out of order. Missing at least 1 image assignment.");
        return;
    }
    int height = art1->height();
    int width = art1->width();
    int channels = art1->channels();
    btrgb::pixel* abitmap1 = art1->bitmap();
    btrgb::pixel* abitmap2 = art2->bitmap();
    btrgb::pixel* wbitmap1 = white1->bitmap();
    btrgb::pixel* wbitmap2 = white2->bitmap();
    btrgb::pixel* dbitmap1 = dark1->bitmap();
    btrgb::pixel* dbitmap2 = dark2->bitmap();
    //Need to pull the channel 2 value from the art white patch,
    //and the corrisponding spot from the white image for these values
    //y value will tell us which patch to look at
    //ONLY NEED CHANNEL 2, aka Green channel, to get averages, y is from calc
    //NEED TO CHANGE, SHOULD ONLY BE FINDING AVG WITHIN THE WHITE PATCH, LOC WILL BE PROVIDED BY ART OBJ
    //Provided from Art Obj
    //Determines how many pixels are compared, 
    //size - 1 = how many rings around the center point to be compared for avg
    int size;
    //Provided from Singleton
    int patchX;
    int patchY;

    float art1Total = 0;
    float white1Total = 0;
    float art2Total = 0;
    float white2Total = 0;
    int x, y, i, ix, iy;
    //May need to be looked at, old implementation of bitmap for loop
    for (y = 0; y < height; y++) {
        iy = y * width * channels;
        for (x = 0; x < width; x++) {
            ix = x * channels;
            //channel will always be index 1, only channel 2 aka green
            i = iy + ix + 1;
            //Need to find out if this is referneced correctly, is ix the x position
            if (ix == patchX || ix == patchX - (size - 1) || ix == patchX + (size - 1)) {
                if (iy == patchY || iy == patchY - (size - 1) || iy == patchY + (size - 1)) {
                    art1Total += abitmap1[i];
                    white1Total += wbitmap1[i];
                    art2Total += abitmap2[i];
                    white2Total += wbitmap2[i];
                }
            }
        }        
    }
    float art1Avg = art1Total/(size*size);
    float white1Avg = white1Total/(size*size);
    float art2Avg = art2Total / (size * size);
    float white2Avg = white2Total / (size * size);
    //Fake data being used here for inputed data, but should be the correct y calculation
    //Real data requires read in of csv file
    //Y VALUE WILL BE PROVIDED, depricate everything beind done for the sole purpose of getting y
    float yVal;
    float w1 = yVal * (white1Avg / art1Avg);
    float w2 = yVal * (white2Avg / art2Avg);
    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted
    //Old version of the For Loop, may need to be changed
    int ch;
    float wPix, dPix, aPix;
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
    sleep_for(seconds(1));
}