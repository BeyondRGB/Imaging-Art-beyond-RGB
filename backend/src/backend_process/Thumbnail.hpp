#ifndef THUMBNAIL_HPP
#define THUMBNAIL_HPP

#include <cppcodec/base64_rfc4648.hpp>

#include "ImageUtil/ImageReader/LibRawThumbnail.hpp"
#include "ImageUtil/Image.hpp"

#include "backend_process.hpp"

class Thumbnail : public BackendProcess {

public:
    Thumbnail();
    ~Thumbnail();
	void run() override;

private:
	static unsigned int id;

};

#endif