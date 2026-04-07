#ifndef BEYOND_RGB_BACKEND_FLATFIELDOR_H
#define BEYOND_RGB_BACKEND_FLATFIELDOR_H

#include <image_processing/LeafComponent.h>
#include <image_util/Image.hpp>

class FlatFieldor : public LeafComponent {
  private:
    float w;
    void wCalc(float pAvg, float wAvg, double yRef);
    /**
     * @brief creates threads linked to the operation of flat fielding
     * @param threads array of threads. this is allocated memory FREE IT
     * @param a: art1 image
     * @param wh: white1 image
     * @param d : dark1 image
     * @param ac : art copy image
     * @param height: height of image
     * @param width: width of image
     * @param channels: channel count
     */
    void create_threads(std::thread **threads, btrgb::Image *a,
                        btrgb::Image *wh, btrgb::Image *d, btrgb::Image *ac,
                        int height, int width, int channels);

  public:
    FlatFieldor() : LeafComponent("Flat Fielding") {}
    void execute(CommunicationObj *comms, btrgb::ArtObject *images) override;
    void store_results(btrgb::ArtObject *images);
};

#endif // BEYOND_RGB_BACKEND_FLATFEILDOR_H
