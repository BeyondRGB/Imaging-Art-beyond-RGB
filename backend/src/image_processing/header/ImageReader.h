#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include "ImageUtil/ImageReader/ImageReaderStrategy.hpp"
#include "ImgProcessingComponent.h"

class ImageReader: public ImgProcessingComponent {

    public:
        enum reader_strategy {RAW_LibRaw, TIFF_OpenCV};
        ImageReader(reader_strategy strategy);
        ~ImageReader();
        void execute(CallBackFunction func, btrgb::ArtObject* images) override;

    private:
        btrgb::ImageReaderStrategy* _reader;

};


#endif //BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
