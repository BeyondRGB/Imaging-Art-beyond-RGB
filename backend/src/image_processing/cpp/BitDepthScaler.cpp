#include <sstream>

#include "image_processing/header/BitDepthScaler.h"


BitDepthScaler::BitDepthScaler() : LeafComponent("BitDepthScaler") {}
BitDepthScaler::~BitDepthScaler() {}


void BitDepthScaler::execute(CommunicationObj* comms, btrgb::ArtObject* images) {

    double total = images->imageCount();
    double count = 0;
    comms->send_progress(0, this->get_name());
    for(const auto& [key, im] : *images) {
        int raw_bd = *(im->_raw_bit_depth);

        /* Output message. */
        std::stringstream out3;
        comms->send_info(out3.str(), this->get_name());
        out3 << "Scaling \"" << im->getName() << "\" from " << raw_bd << " to 16 bits...";


        /* If the bit depth is invalid or already 16 bits: skip, do not scale anything. */
        if (raw_bd < 16 && raw_bd >= 8) {

            /* Math:
            * scaler = (2^16 - 1) / (2^bit_depth - 1)
            *
            * The value of two to any power can be performed with bit shifting:
            * 2^x = 1 << x
            */
            float scaler = float( (1 << 16) - 1 ) / float( (1 << raw_bd) - 1);

            /* Multiply each element in matrix by the scaler. */
            im->getMat() *= scaler;
        }


        count++;
        comms->send_progress(count/total, this->get_name());
    }

}

