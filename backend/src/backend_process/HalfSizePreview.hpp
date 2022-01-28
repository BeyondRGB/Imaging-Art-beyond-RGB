#ifndef HALFSIZEPREVIEW_H
#define HALFSIZEPREVIEW_H

#include <vector>
#include <stdint.h>

#include "ImageUtil/Image.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"
#include "backend_process.hpp"
#include "image_processing/header/LibRawReader.h"
#include "image_processing/header/ManualBitDepthFinder.h"
#include "ImageUtil/ImageWriter/LibpngWriter.hpp"


class HalfSizePreview : public BackendProcess {

public:
    HalfSizePreview();
    ~HalfSizePreview();
	void run() override;

private:
	static unsigned int id;
    typedef std::vector<btrgb::image*> image_vector;
    struct Png {
        std::string filename;
        std::vector<uint8_t> binary;
    };
    typedef std::vector<Png> png_image_vector;

    //void send

};


#endif
