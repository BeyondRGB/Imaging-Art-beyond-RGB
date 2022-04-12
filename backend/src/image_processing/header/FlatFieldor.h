#ifndef BEYOND_RGB_BACKEND_FLATFIELDOR_H
#define BEYOND_RGB_BACKEND_FLATFIELDOR_H

#include "image_processing/header/LeafComponent.h"
#include "ImageUtil/Image.hpp"
#include "image_processing/results/calibration_results.hpp"

class FlatFieldor : public LeafComponent{
private:
    float w;
    void wCalc(float pAvg, float wAvg, double yRef);
    void pixelOperation(int h, int wid, int c, btrgb::Image* a, btrgb::Image* wh, btrgb::Image* d, btrgb::Image* ac);

public:
    FlatFieldor() : LeafComponent("Flat Fielding"){}
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    void store_results(btrgb::ArtObject* images);
};

#endif //BEYOND_RGB_BACKEND_FLATFEILDOR_H
