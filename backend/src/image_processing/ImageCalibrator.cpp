//
// Created by ThinkPad41 on 10/10/2021.
//

#include <image_processing/CompositComponent.h>
#include <image_processing/ImageCalibrator.h>
#include <image_processing/ImgProcessingComponent.h>
#include <vector>

ImageCalibrator::ImageCalibrator(
    const std::vector<std::shared_ptr<ImgProcessingComponent>> &components)
    : CompositComponent("Profiler") {
    this->init_components(components);
}

void ImageCalibrator::execute(CommunicationObj *comms,
                              btrgb::ArtObject *images) {
    comms->send_info("Starting Image Calibration", "Image Calibration");
    double count = 0;
    double total = this->components.size();
    for (auto &component : this->components) {
        double currProgress = count / total;
        comms->send_progress(currProgress, this->get_name());
        component->execute(comms, images);
        comms->send_binary(images->getImage("ColorManaged"), btrgb::FAST);
        count++;
    }
    comms->send_info("Image Calibration Done!!!", this->get_name());
}
