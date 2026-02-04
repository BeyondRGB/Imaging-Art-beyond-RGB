#ifndef TIFFWRITER_H
#define TIFFWRITER_H

extern "C" {
#include <tiffio.h>
}
#include <jsoncons/json.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#include <image_util/Image.hpp>
#include <image_util/image_writer/ImageWriterStrategy.hpp>

namespace btrgb {

class LibTiffWriter : public ImageWriterStrategy {
  public:
    LibTiffWriter();
    ~LibTiffWriter();

  protected:
    void _write(Image *im, std::string filename) override;

  private:
    std::string getCustomTag(Image *im);
};

class LibTiff_OpenFileFailed : public ImageWritingError {
  public:
    virtual char const *what() const noexcept {
        return "LibTiff Error: Cannot open file for writing.";
    }
};

class LibTiff_WriteStripFailed : public ImageWritingError {
  public:
    virtual char const *what() const noexcept {
        return "LibTiff Error: Cannot write strip buffer to file.";
    }
};
} // namespace btrgb

#endif