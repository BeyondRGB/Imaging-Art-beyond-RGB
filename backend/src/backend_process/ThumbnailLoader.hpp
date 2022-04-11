#ifndef THUMBNAILLOADER_HPP
#define THUMBNAILLOADER_HPP

#include "ImageUtil/ImageReader/LibTiffReader.hpp"
#include "ImageUtil/ImageReader/LibRawThumbnail.hpp"
#include "ImageUtil/Image.hpp"

#include "backend_process.hpp"

class ThumbnailLoader : public BackendProcess {

public:
    ThumbnailLoader(std::string name) : BackendProcess(name) {};
    ~ThumbnailLoader();
	void run() override;

private:
    void _read_raw_thumbnail(btrgb::LibRawThumbnail* reader, std::string file);
    void _read_tiff(btrgb::LibTiffReader* reader, std::string file);
};

#endif