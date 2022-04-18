#include "../header/PreProcessor.h"

PreProcessor::PreProcessor(const std::vector<std::shared_ptr<ImgProcessingComponent>>& components)
    : CompositComponent("Pre-processor") {
        this->init_components(components);
}

void PreProcessor::execute(CommunicationObj* comms, btrgb::ArtObject* images) {
    comms->send_info("Starting PreProcessing", this->get_name());
    double count = 0;
    double total = this->components.size();
    for(const auto& component : this->components){
      double currProgress = count / total;
      comms->send_progress(currProgress, this->get_name());
      component->execute(comms, images);
      comms->send_binary(images->getImage("art1"), btrgb::FAST);
      count++;
    }
    comms->send_info("PreProcessing Done!!!", this->get_name());
}
