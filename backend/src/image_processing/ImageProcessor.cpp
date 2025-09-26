#include <image_processing/ImageProcessor.h>

ImageProcessor::ImageProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components)
    : CompositComponent("Image Processor") {
        this->init_components(components);
}
void ImageProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting Image Processor", this->get_name());
    double count = 0;
    double total = this->components.size();
    for(auto  & component : this->components){
        double currProgress = count / total;
        comms->send_progress(currProgress, this->get_name());
        component->execute(comms, images);
        count++;
    }
    comms->send_info("Image Processing Done!!!", this->get_name());
}
