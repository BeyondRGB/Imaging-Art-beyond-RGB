#include "../header/ImageProcessor.h"

ImageProcessor::ImageProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>> &components)
    : CompositComponent("ImageProcessor") {
        this->init_components(components);
}
void ImageProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting Image Processor", "ImageProcessor");
    double count = 0;
    double total = this->components.size();
    for(auto  & component : this->components){
        double currProgress = count / total;
        comms->send_progress(currProgress, "ImageProcessor");
        component->execute(comms, images);
        count++;
    }
    comms->send_info("Image Processing Done!!!", "ImageProcessor");
}

// std::string ImageProcessor::get_component_list(){
//     std::string component_list = "{\"ImageProcessor\":[";
//     int i = 0;
//     for(auto  & component : this->components){
//         if(i > 0){
//             component_list += ",";
//         }
//         component_list += component->get_component_list();
//         i++;
//     }
//     component_list += "]}";
//     return component_list;
// }
