#include <sstream>

#include "image_processing/header/BitDepthScaler.h"
#include "image_processing/header/CvMatScaleBD.h"


BitDepthScaler::BitDepthScaler(std::string strategy) {
    if( strategy == "OpenCV Matrix forEach" ) {
        this->strategy = new CvMatScaleBD();
    }
    /* Default to manual. */
    else {
        this->strategy = new CvMatScaleBD();
    }
}


BitDepthScaler::~BitDepthScaler() {
    delete this->strategy;
}


# define PRINT_MSG_3(callb, fname, bd) \
do { \
    std::stringstream out3; \
    out3 << "Scaling \"" << fname << "\" from " << bd << " to 16 bits..."; \
    callb(out3.str()); \
} while(0)


void BitDepthScaler::execute(CallBackFunction func, btrgb::ArtObject* images) {

    for(const auto& [key, im] : *images) {

        PRINT_MSG_3(func, im->filename(), im->_raw_bit_depth);
        this->strategy->scale(im);

        images->outputImageAs(btrgb::TIFF, key, key + "_bd_scaled");
    }
    
}
#undef PRINT_MSG_3