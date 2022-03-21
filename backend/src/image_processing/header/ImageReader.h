#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include "ImageUtil/ImageReader/ImageReaderStrategy.hpp"
#include "ImgProcessingComponent.h"

class ImageReader: public ImgProcessingComponent {

    public:
        enum reader_strategy {none, RAW_LibRaw, TIFF_OpenCV};
        ImageReader();
        ~ImageReader();
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

    private:
        reader_strategy _current_strategy = reader_strategy::none;
        btrgb::ImageReaderStrategy* _reader = nullptr;
        void _set_strategy(reader_strategy strategy);

};


#endif //BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
