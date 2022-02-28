#ifndef THUMBNAIL_HPP
#define THUMBNAIL_HPP

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

}

#endif