#ifndef BTRGB_LIBRAWREADER_HPP
#define BTRGB_LIBRAWREADER_HPP

#include <libraw.h>
#include <math.h>
#include <memory>

#include "ImageReaderStrategy.hpp"

namespace btrgb {

class LibRawReader : public ImageReaderStrategy {

  public:
    enum libraw_type { PREVIEW, UNPROCESSED };
    LibRawReader(libraw_type method = UNPROCESSED);
    ~LibRawReader();

    void open(std::string filename) override;
    void recycle() override;

    void copyBitmapTo(void *buffer, uint32_t size) override;
    void copyBitmapTo(cv::Mat &im) override;

  private:
    LibRaw _reader;
    void _configLibRawParams();
    enum libraw_type _method = UNPROCESSED;
    void _error(std::string msg);
};

class LibRawFileTypeUnsupported : public ImageReaderStrategyError {
  public:
    virtual char const *what() const noexcept {
        return "File type unknown, or unsupported by LibRaw.";
    }
};

} // namespace btrgb

#endif