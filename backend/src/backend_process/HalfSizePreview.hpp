#ifndef HALFSIZEPREVIEW_H
#define HALFSIZEPREVIEW_H

#include <vector>
#include <stdint.h>
#include <cppcodec/base64_rfc4648.hpp>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/LibpngWriter.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"
#include "backend_process.hpp"
#include "image_processing/header/LibRawReader.h"
#include "image_processing/header/ManualBitDepthFinder.h"



class HalfSizePreview : public BackendProcess {

public:
    HalfSizePreview();
    ~HalfSizePreview();
	void run() override;

private:
	static unsigned int id;

};


#endif
