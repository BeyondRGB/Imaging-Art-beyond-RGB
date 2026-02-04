#ifndef BTRGB_SPECTRALPICKER_HPP
#define BTRGB_SPECTRALPICKER_HPP

#include <fstream>
#include <jsoncons/json.hpp>

#include <image_util/ColorProfiles.hpp>
#include <image_util/Image.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <server/communication_obj.hpp>
#include <utils/json.hpp>

#include "backend_process.hpp"

class SpectralPicker : public BackendProcess {

  public:
    SpectralPicker(std::string name) : BackendProcess(name) {};
    ~SpectralPicker();
    void run() override;
};

#endif
