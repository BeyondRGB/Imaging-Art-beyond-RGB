#ifndef BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
#define BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H


// #include "ImgProcessingComponent.h"
#include "image_processing/header/CompositComponent.h"

class ImageProcessor: public CompositComponent{
private:
    // std::vector<std::shared_ptr<ImgProcessingComponent>> components;
public:
    ImageProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components);
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    // std::string get_component_list() override;
};


#endif //BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
