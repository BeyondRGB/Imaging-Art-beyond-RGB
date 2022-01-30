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

    btrgb::image* white1;
    try {
        white1 = images->getImage("white1");
    } catch( const btrgb::ArtObj_ImageDoesNotExist&  e ) {
        func("Error: Bit depth scaling called out of order. Missing \"white1\" image assignment.");
        return;
    }
    PRINT_MSG_1(func, white1->filename());
    bit_depth = this->strategy->get_bit_depth(white1);
    PRINT_MSG_2(func, white1->filename(), bit_depth);

    for(const auto& [key, im] : *images) {

        PRINT_MSG_3(func, im->filename(), bit_depth);
        this->strategy->scale(im, bit_depth);
    }

    //Outputs TIFFs for each image group for after this step, temporary
    btrgb::image* img1 = images->getImage("art1");
    btrgb::image* img2 = images->getImage("art2");
    img1->setFilename("BitDepthScaleingOut1");
    img2->setFilename("BitDepthScaleingOut2");
    images->outputImageAsTIFF("art1");
    images->outputImageAsTIFF("art2");
    
}


#undef PRINT_MSG_1
#undef PRINT_MSG_2
#undef PRINT_MSG_3