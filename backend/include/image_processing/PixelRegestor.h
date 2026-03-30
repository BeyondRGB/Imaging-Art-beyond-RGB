
#ifndef BEYOND_RGB_BACKEND_PIXELREGESTOR_H
#define BEYOND_RGB_BACKEND_PIXELREGESTOR_H

#include <image_processing/LeafComponent.h>
#include <image_util/Image.hpp>

class PixelRegestor : public LeafComponent {
  private:
    std::string RegistrationFactor;

  public:
    ~PixelRegestor() {};
    PixelRegestor(std::string RegistrationFactor)
        : LeafComponent("Registering"),
          RegistrationFactor(RegistrationFactor) {};
    void execute(CommunicationObj *comms, btrgb::ArtObject *images) override;
};

#endif // BEYOND_RGB_BACKEND_PIXELREGESTOR_H