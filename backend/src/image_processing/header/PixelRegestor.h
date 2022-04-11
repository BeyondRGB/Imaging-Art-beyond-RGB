
#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H

#include "image_processing/header/LeafComponent.h"

class PixelRegestor : public LeafComponent{
private: std::string RegistrationFactor;
public:
    ~PixelRegestor() {};
    PixelRegestor(std::string RegistrationFactor) : LeafComponent("Pixel Registration"), RegistrationFactor(RegistrationFactor) {};
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};

#endif //BEYOND_RGB_BACKEND_PIXELREGESTOR_H