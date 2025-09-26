#ifndef HALFSIZEPREVIEW_H
#define HALFSIZEPREVIEW_H

#include <image_util/Image.hpp>
#include <image_util/image_reader/LibRawReader.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_util/image_reader/ImageReaderStrategy.hpp>
#include <utils/json.hpp>
#include <server/communication_obj.hpp>

#include "backend_process.hpp"



class HalfSizePreview : public BackendProcess {

public:
    HalfSizePreview(std::string name) : BackendProcess(name) {};
    ~HalfSizePreview();
	void run() override;

};


#endif
