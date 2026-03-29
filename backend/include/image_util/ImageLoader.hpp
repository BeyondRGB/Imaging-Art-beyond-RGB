#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <image_util/Image.hpp>
#include <image_processing/LeafComponent.h>
#include <image_util/image_reader/ImageReaderStrategy.hpp>
#include <image_util/ImageLoader.hpp>
#include <image_util/BitDepthFinder.hpp>

class ImageLoader {

  public:
    // exists for all instances, used to update comms
    static std::mutex comms_mutex;
    enum reader_strategy { none, RAW_LibRaw, TIFF_OpenCV, TIFF_LibTiff };
    ImageLoader(CommunicationObj *comms, btrgb::ArtObject *images, std::string name,
                std::string key, btrgb::Image *im, btrgb::BitDepthFinder *util,
                std::shared_ptr<int> bit_depth, int total_images);
    ~ImageLoader();
    void load_image();
    
  private:

    CommunicationObj *comms;
    btrgb::ArtObject *images; 
    std::string name;
    std::string key;
    btrgb::Image *im;
    btrgb::BitDepthFinder *util;
    std::shared_ptr<int> bit_depth;
    reader_strategy _current_strategy = reader_strategy::none;
    btrgb::ImageReaderStrategy *_reader = nullptr;
    int total_images;
    
    void _set_strategy(reader_strategy strategy);
    void _average_greens(cv::Mat &input, cv::Mat &output);
};

#endif // IMAGE_LOADER_H
