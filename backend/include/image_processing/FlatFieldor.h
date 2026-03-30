#ifndef BEYOND_RGB_BACKEND_FLATFIELDOR_H
#define BEYOND_RGB_BACKEND_FLATFIELDOR_H

#include <image_processing/LeafComponent.h>
#include <image_util/Image.hpp>

class FlatFieldor : public LeafComponent {
  private:
    float w;
    void wCalc(float pAvg, float wAvg, double yRef);

  public:
    FlatFieldor() : LeafComponent("Flat Fielding") {}
    void execute(CommunicationObj *comms, btrgb::ArtObject *images) override;
    void store_results(btrgb::ArtObject *images);
};

#endif // BEYOND_RGB_BACKEND_FLATFEILDOR_H
