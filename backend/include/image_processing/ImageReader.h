#ifndef BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
#define BEYOND_RGB_BACKEND_RAWIMAGEREADER_H

#include <image_processing/LeafComponent.h>
#include <image_util/ImageLoader.hpp>
#include <image_util/image_reader/ImageReaderStrategy.hpp>
#include <vector>

class ImageReader : public LeafComponent {

  public:
    ImageReader();
    ~ImageReader();
    void execute(CommunicationObj *comms, btrgb::ArtObject *images) override;

  private:
    std::vector<ImageLoader *> _loaders;
};

#endif // BEYOND_RGB_BACKEND_RAWIMAGEREADER_H
