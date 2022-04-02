#ifndef BEYOND_RGB_BACKEND_BITDEPTHSCALER_H
#define BEYOND_RGB_BACKEND_BITDEPTHSCALER_H

#include "image_processing/header/LeafComponent.h"

class BitDepthScaler : public LeafComponent {

    public:
        BitDepthScaler();
        ~BitDepthScaler();
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};


#endif
