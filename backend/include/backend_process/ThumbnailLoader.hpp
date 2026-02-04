#ifndef THUMBNAILLOADER_HPP
#define THUMBNAILLOADER_HPP

#include <image_util/Image.hpp>
#include <image_util/image_reader/LibRawThumbnail.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>

#include "backend_process.hpp"

class ThumbnailLoader : public BackendProcess {

  public:
    ThumbnailLoader(std::string name) : BackendProcess(name) {};
    ~ThumbnailLoader();
    void run() override;

  private:
    void _read_raw_thumbnail(btrgb::LibRawThumbnail *reader, std::string file);
    void _read_tiff(btrgb::LibTiffReader *reader, std::string file);
};

#endif