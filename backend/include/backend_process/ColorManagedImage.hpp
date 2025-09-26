#ifndef BTRGB_VIEWTIFF_HPP
#define BTRGB_VIEWTIFF_HPP

#include <fstream>
#include <jsoncons/json.hpp>

#include <image_util/Image.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_util/ColorProfiles.hpp>
#include <utils/json.hpp>
#include <server/communication_obj.hpp>

#include "backend_process.hpp"



class ColorManagedImage : public BackendProcess {

public:
    ColorManagedImage(std::string name) : BackendProcess(name) {};
    ~ColorManagedImage();
	void run() override;

};


#endif
