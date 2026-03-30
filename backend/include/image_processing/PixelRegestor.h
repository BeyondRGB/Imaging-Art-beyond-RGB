
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

  private:
    /**
     * @brief the threading results in images being generated in parts, this stitches
     *  them back together and then sends them to the client
     * @param comms the reference to the communications object to send data through
     * @param output the path of where the images should be saved
     * @param matchfloat this is the matrix output pixels are saved to
     */
    void send_image(CommunicationObj *comms, std::string output, cv::Mat matchfloat);
};

#endif // BEYOND_RGB_BACKEND_PIXELREGESTOR_H