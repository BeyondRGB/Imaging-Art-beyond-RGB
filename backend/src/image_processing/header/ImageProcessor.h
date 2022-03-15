//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
#define BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H


#include "ImgProcessingComponent.h"

class ImageProcessor: public ImgProcessingComponent{
private:
    std::vector<std::shared_ptr<ImgProcessingComponent>> components;
public:
    ImageProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components);
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
};


#endif //BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
