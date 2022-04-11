
#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H

#include "image_processing/header/LeafComponent.h"

class PixelRegestor : public LeafComponent{
public:
    PixelRegestor() : LeafComponent("Pixel Registration"){}
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};

#endif //BEYOND_RGB_BACKEND_PIXELREGESTOR_H
