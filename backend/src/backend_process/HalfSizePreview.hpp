#ifndef HALFSIZEPREVIEW_H
#define HALFSIZEPREVIEW_H

#include <vector>
#include <stdint.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cppcodec/base64_rfc4648.hpp>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/RawReading/HalfSizeReader.hpp"
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
