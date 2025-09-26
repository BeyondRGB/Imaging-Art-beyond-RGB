#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <image_processing/LeafComponent.h>

// #include <lcms2.h>

class Verification : public LeafComponent{

    public:
        Verification() : LeafComponent("Verifying"){};
        ~Verification(){};
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

        void verify_CM_calibration(CommunicationObj* comms, btrgb::ArtObject* images);
        void verify_SP_calibration(CommunicationObj* comms, btrgb::ArtObject* images);

    private:
        ColorTarget target1;
        ColorTarget target2;
        RefData* verification_data;
        int channel_count;


};

#endif //VERIFICATION_H