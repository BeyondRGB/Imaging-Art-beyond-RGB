#ifndef BEYOND_RGB_BACKEND_IMAGECALIBRATOR_H
#define BEYOND_RGB_BACKEND_IMAGECALIBRATOR_H

#include <image_processing/CompositComponent.h>
#include <image_processing/ImgProcessingComponent.h>

#include <image_util/ArtObject.hpp>

#include <server/communication_obj.hpp>

class ImageCalibrator : public CompositComponent {

  public:
    ImageCalibrator(
        const std::vector<std::shared_ptr<ImgProcessingComponent>> &components);
    void execute(CommunicationObj *comms, btrgb::ArtObject *images);
};

#endif // BEYOND_RGB_BACKEND_IMAGECALIBRATOR_H
