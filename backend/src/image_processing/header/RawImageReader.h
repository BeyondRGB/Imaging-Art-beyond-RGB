#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include "ImgProcessingComponent.h"
#include "image_processing/header/RawReaderStrategy.h"

class RawImageReader: public ImgProcessingComponent {

    public:
        RawImageReader();
        RawImageReader(std::string strategy);
        ~RawImageReader();
        void execute(CallBackFunction func, btrgb::ArtObject* images) override;

    private:
        RawReaderStrategy* fileReader;

};


#endif //BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
