#include <string>

#include <image_processing/ImageReader.h>
#include <image_util/BitDepthFinder.hpp>
#include <image_util/Image.hpp>
#include <image_util/image_reader/LibRawReader.hpp>
#include <image_util/image_reader/LibTiffReader.hpp>
#include <image_util/image_reader/TiffReaderOpenCV.hpp>
#include <utils/threading_statics/image_reader_static.hpp>

ImageReader::ImageReader() : LeafComponent("Reading") {}

ImageReader::~ImageReader() {}

void ImageReader::execute(CommunicationObj *comms, btrgb::ArtObject *images) {
    comms->send_info("Reading In Raw Image Data!", this->get_name());

    btrgb::BitDepthFinder util;
    std::shared_ptr<int> bit_depth(new int(-1));

    double total = images->imageCount();
    double count = 0;
    std::vector<std::thread *> threads;
    comms->send_progress(0, this->get_name());
    for (const auto &[key, im] : *images) {
        ImageLoader *l = new ImageLoader(comms, images, this->get_name(), key, im, &util, bit_depth);
        this->_loaders.push_back(l);
        threads.push_back(new std::thread(btrgb::imagereader::load_image, l));
    }
    // join threads
    for (std::thread *t : threads)
    {
        t->join();
        count++;
        comms->send_progress(count / total, this->get_name());
        delete t;
    }

    // free up the loaders
    for (ImageLoader *l : this->_loaders)
        delete l;

    comms->send_progress(1, this->get_name());
}
