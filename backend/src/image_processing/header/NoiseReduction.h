
#ifndef BEYOND_RGB_BACKEND_NOISEREDUCTION_H
#define BEYOND_RGB_BACKEND_NOISEREDUCTION_H


#include "image_processing/header/LeafComponent.h"

class NoiseReduction : public LeafComponent {
private: std::string SharpenFactor;
public:
    ~NoiseReduction() {};
    NoiseReduction(std::string SharpenFactor) : LeafComponent("Noise Reduction"), SharpenFactor(SharpenFactor) {};
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_NOISEREDUCTION_H
