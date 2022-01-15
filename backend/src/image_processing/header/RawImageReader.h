#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include "ImgProcessingComponent.h"
#include "image_processing/header/RawFileReader.h"

class RawImageReader: public ImgProcessingComponent {

    public:
        RawImageReader(std::string strategy);
        ~RawImageReader();
        void execute(CallBackFunction func, btrgb::ArtObject* images) override;

    private:
        RawFileReader* fileReader;

};


#endif //BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
