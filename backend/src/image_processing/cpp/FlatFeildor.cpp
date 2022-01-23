//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/FlatFeildor.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

void FlatFeildor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Flat Fielding");
    //First need to read in data from a csv format, prtend a file is provided
    //Work in progress, priority on fucntion of flatfielding
 /*   string fileName;
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    fstream file(fileName, ios::in);
    if (file.is_open()) {
        //Cycle for each meaningful row in the file
    }*/
    
    int height = im->height();
    int width = im->width();
    int channels = im->channels();
    btrgb::pixel* bitmap = im->bitmap();
    //Need to pull the channel 2 value from the art white patch,
    //and the corrisponding spot from the white image for these values
    //y value will tell us which patch to look at
    //ONLY NEED CHANNEL 2, aka Green channel, to get averages, y is from calc
    float count = 0;
    float artTotal = 0;
    float whiteTotal = 0;
    int x, y, i, ix, iy;
    for (y = 0; y < height; y++) {
        iy = y * width * channels;
        for (x = 0; x < width; x++) {
            ix = x * channels;
            //channel will always be index 1
            i = iy + ix + 1;
            //Need to find out if this is referneced correctly
            //Add up art pixel channel 2 values FAKE RN
            artTotal += art[i];
            //Add up white pixel channel 2 values FAKE RN
            whiteTotal += white[i];
            //add 1 to count, will be used to get avg later
            count += 1;
        }        
    }
    float artAvg = artTotal/count;
    float whiteAvg = whiteTotal/count;
    //Fake data being used here for inputed data, but should be the correct y calculation
    //Real data requires read in of csv file
    float reflectance[] = {};
    float source[] = {};
    float obsv[] = {};
    float allSum = 0;
    float soSum = 0;
    for (int i = 0; i < reflectance.size(); i++) {
        allSum += reflectance[i] * obsv[i] * source[i];
        soSum += obsv[i] * source[i];
    }
    float yVal = (100*allSum)/soSum;
    float w = yVal * (whiteAvg / artAvg);
    //For loop is for every pixel in the image, and gets a corrisponding pixel from white and dark images
    //Every Channel value for each pixel needs to be adjusted
    //Need to change based on Image.hpp
    int ch;
    float wPix, dPix, aPix;
    for (y = 0; y < height; y++) {
        iy = y * width * channels;
        for (x = 0; x < width; x++) {
            ix = x * channels;
            for (ch = 0; ch < channels; ch++) {
                i = iy + ix + ch;
                // i is the index for the bitmap
                wPix = whiteImage[i];
                dPix = darkImage[i];
                aPix = artImage[i];
                //Need to overwrite previous image pixel in the Art Object
                artImage[i] = w((aPix - dPix) / (wPix - dPix));
            }
        }
    }
    sleep_for(seconds(1));
}