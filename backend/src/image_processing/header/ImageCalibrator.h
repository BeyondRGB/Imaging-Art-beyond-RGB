#ifndef BEYOND_RGB_BACKEND_IMAGECALIBRATOR_H
#define BEYOND_RGB_BACKEND_IMAGECALIBRATOR_H

#include "image_processing/header/CompositComponent.h"

class ImageCalibrator : public CompositComponent{

public:
    ImageCalibrator(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components);
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    
};

#endif //BEYOND_RGB_BACKEND_IMAGECALIBRATOR_H
