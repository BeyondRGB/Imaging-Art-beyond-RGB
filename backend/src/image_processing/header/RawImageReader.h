#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include <math.h>
#include <memory>
#include <libraw.h>

#include "ImgProcessingComponent.h"

class RawImageReader: public ImgProcessingComponent {

private:
    class InternalRawProcessor : public LibRaw {
        public: void custom_process();
    };
    InternalRawProcessor rawReader;
    void configLibRawParams();

public:
    RawImageReader();
    ~RawImageReader();
    void execute(CallBackFunction func, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
