
#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H


// #include "ImgProcessingComponent.h"
#include "image_processing/header/LeafComponent.h"

class PixelRegestor : public LeafComponent{
public:
    PixelRegestor() : LeafComponent("PixelRegestor"){}
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    // std::string get_component_list() override;
};


#endif //BEYOND_RGB_BACKEND_PIXELREGESTOR_H
