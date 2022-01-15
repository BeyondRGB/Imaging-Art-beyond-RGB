#include <sstream>

#include "image_processing/header/BitDepthScaler.h"
#include "image_processing/header/ManualBitDepthFinder.h"


BitDepthScaler::BitDepthScaler() {
    this->strategy = new ManualBitDepthFinder();
}


BitDepthScaler::BitDepthScaler(std::string strategy) {
    if( strategy == "Manual" ) {
        this->strategy = new ManualBitDepthFinder();
    }
    /* Default to manual. */
    else {
        this->strategy = new ManualBitDepthFinder();
    }
}


BitDepthScaler::~BitDepthScaler() {
    delete this->strategy;
}


#define PRINT_MSG_1(callb, fname) \
    do { \
        std::stringstream out1; \
        out1 << "Testing bit depth of \"" << fname << "\"..."; \
        callb(out1.str()); \
    } while(0)
# define PRINT_MSG_2(callb, fname, bd) \
    do { \
        std::stringstream out2; \
        out2 << "Bit depth of \"" << fname << "\" is " << bd << "bits."; \
        callb(out2.str()); \
    } while(0)
# define PRINT_MSG_3(callb, fname, bd) \
    do { \
        std::stringstream out3; \
        out3 << "Scaling \"" << fname << "\" from " << bd << " to 16 bits..."; \
        callb(out3.str()); \
    } while(0)


void BitDepthScaler::execute(CallBackFunction func, btrgb::ArtObject* images) {
    int bit_depth = 0;

    for(const auto& [key, im] : *images) {
        if( ! bit_depth ) {
            PRINT_MSG_1(func, im->filename());
            bit_depth = this->strategy->get_bit_depth(im);
            PRINT_MSG_2(func, im->filename(), bit_depth);
        }

        PRINT_MSG_3(func, im->filename(), bit_depth);
        this->strategy->scale(im, bit_depth);
    }
    
}


#undef PRINT_MSG_1
#undef PRINT_MSG_2
#undef PRINT_MSG_3