#ifndef VERIFICATION_H
#define VERIFICATION_H

#include "image_processing/header/ImgProcessingComponent.h"
#include "utils/calibration_util.hpp"

// #include <lcms2.h>

class Verification : public ImgProcessingComponent{

    public:
        Verification(){};
        ~Verification(){};
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

        void verify_CM_calibration(CommunicationObj* comms, btrgb::ArtObject* images);
        void verify_SP_calibration(CommunicationObj* comms, btrgb::ArtObject* images);


};

#endif //VERIFICATION_H
