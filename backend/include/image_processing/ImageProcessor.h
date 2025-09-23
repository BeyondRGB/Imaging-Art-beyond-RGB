#ifndef BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
#define BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H

#include <image_processing/CompositComponent.h>
#include <image_processing/ImageCalibrator.h>

class ImageProcessor: public CompositComponent{

public:
    ImageProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components);
    void execute(CommunicationObj* comms, btrgb::ArtObject* images);

};

#endif //BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
