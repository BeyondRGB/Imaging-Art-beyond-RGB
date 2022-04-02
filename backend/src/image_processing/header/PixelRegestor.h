
#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H


#include "ImgProcessingComponent.h"

class PixelRegestor : public ImgProcessingComponent{
public:
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    std::string get_component_list() override;
};


#endif //BEYOND_RGB_BACKEND_PIXELREGESTOR_H
