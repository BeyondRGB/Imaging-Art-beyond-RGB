#ifndef BEYOND_RGB_BACKEND_PREPROCESSOR_H
#define BEYOND_RGB_BACKEND_PREPROCESSOR_H

#include <image_processing/CompositComponent.h>


class PreProcessor: public CompositComponent {

public:
    explicit PreProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components);
    void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

};

#endif //BEYOND_RGB_BACKEND_PREPROCESSOR_H
