//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_PREPROCESSOR_H
#define BEYOND_RGB_BACKEND_PREPROCESSOR_H


// #include "ImgProcessingComponent.h"
#include "image_processing/header/CompositComponent.h"


class PreProcessor: public CompositComponent {

private:
    // std::vector<std::shared_ptr<ImgProcessingComponent>> components;

public:
    explicit PreProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components);
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;
    // std::string get_component_list() override;

};


#endif //BEYOND_RGB_BACKEND_PREPROCESSOR_H
