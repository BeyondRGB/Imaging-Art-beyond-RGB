
#ifndef BEYOND_RGB_BACKEND_NOISEREDUCTION_H
#define BEYOND_RGB_BACKEND_NOISEREDUCTION_H


#include "ImgProcessingComponent.h"

class NoiseReduction : public ImgProcessingComponent {
public:
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_NOISEREDUCTION_H
