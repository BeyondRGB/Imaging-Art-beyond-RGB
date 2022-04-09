
#ifndef BEYOND_RGB_BACKEND_NOISEREDUCTION_H
#define BEYOND_RGB_BACKEND_NOISEREDUCTION_H


#include "ImgProcessingComponent.h"

class NoiseReduction : public ImgProcessingComponent {
private: std::string SharpenFactor;
public:
    NoiseReduction(std::string SharpenFactor) :SharpenFactor(SharpenFactor) {};
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_NOISEREDUCTION_H
