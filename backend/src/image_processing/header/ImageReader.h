#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include "ImageUtil/ImageReader/ImageReaderStrategy.hpp"
#include "image_processing/header/LeafComponent.h"

class ImageReader: public LeafComponent {

    public:
        enum reader_strategy {none, RAW_LibRaw, TIFF_OpenCV, TIFF_LibTiff};
        ImageReader();
        ~ImageReader();
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

    private:
        reader_strategy _current_strategy = reader_strategy::none;
        btrgb::ImageReaderStrategy* _reader = nullptr;
        void _set_strategy(reader_strategy strategy);
        void _average_greens(cv::Mat& input, cv::Mat& output);


};

#endif //BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
