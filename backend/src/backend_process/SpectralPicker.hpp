#ifndef BTRGB_SPECTRALPICKER_HPP
#define BTRGB_SPECTRALPICKER_HPP

#include <fstream>
#include <jsoncons/json.hpp>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageReader/LibTiffReader.hpp"
#include "ImageUtil/ColorProfiles.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"

#include "backend_process.hpp"



class SpectralPicker : public BackendProcess {

public:
    SpectralPicker(std::string name) : BackendProcess(name) {};
    ~SpectralPicker();
	void run() override;

};


#endif
