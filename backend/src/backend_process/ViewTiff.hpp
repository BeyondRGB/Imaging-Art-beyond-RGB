#ifndef BTRGB_VIEWTIFF_HPP
#define BTRGB_VIEWTIFF_HPP

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageReader/LibTiffReader.hpp"
#include "ImageUtil/ColorProfiles.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"

#include "backend_process.hpp"



class ViewTiff : public BackendProcess {

public:
    ViewTiff();
    ~ViewTiff();
	void run() override;

private:
	static unsigned int id;

};


#endif
