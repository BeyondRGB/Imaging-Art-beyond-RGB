#ifndef THUMBNAILLOADER_HPP
#define THUMBNAILLOADER_HPP

#include <cppcodec/base64_rfc4648.hpp>

#include "ImageUtil/ImageReader/LibRawThumbnail.hpp"
#include "ImageUtil/Image.hpp"

#include "backend_process.hpp"

class ThumbnailLoader : public BackendProcess {

public:
    ThumbnailLoader();
    ~ThumbnailLoader();
	void run() override;

private:
	static unsigned int id;

};

#endif