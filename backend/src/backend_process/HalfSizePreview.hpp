#ifndef HALFSIZEPREVIEW_H
#define HALFSIZEPREVIEW_H

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageReader/LibRawReader.hpp"
#include "ImageUtil/ImageReader/LibTiffReader.hpp"
#include "ImageUtil/ImageReader/ImageReaderStrategy.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"

#include "backend_process.hpp"



class HalfSizePreview : public BackendProcess {

public:
    HalfSizePreview();
    ~HalfSizePreview();
	void run() override;

private:
	static unsigned int id;

};


#endif
