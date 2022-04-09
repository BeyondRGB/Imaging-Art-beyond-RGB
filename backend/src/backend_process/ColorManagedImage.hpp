#ifndef BTRGB_VIEWTIFF_HPP
#define BTRGB_VIEWTIFF_HPP

#include <fstream>
#include <jsoncons/json.hpp>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageReader/LibTiffReader.hpp"
#include "ImageUtil/ColorProfiles.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"

#include "backend_process.hpp"



class ColorManagedImage : public BackendProcess {

public:
    ColorManagedImage(std::string name) : BackendProcess(name) {};
    ~ColorManagedImage();
	void run() override;

};


#endif
