#ifndef HALFSIZEPREVIEW_H
#define HALFSIZEPREVIEW_H

#include <vector>

#include "ImageUtil/Image.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"
#include "backend_process.hpp"
#include "image_processing/header/LibRawReader.h"


class HalfSizePreview : public BackendProcess {

public:
    HalfSizePreview();
    ~HalfSizePreview();
	void run() override;

private:
	static unsigned int id;
    typedef std::vector<btrgb::image*> image_vector;
    typedef std::vector<btrgb::image*> png_image_vector;
    void initImageObjects(image_vector& images);
    void loadRawImages(image_vector& images);
    void deleteImageObjects(image_vector& images);
    //void send

};


#endif
