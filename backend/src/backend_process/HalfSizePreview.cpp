#include "backend_process/HalfSizePreview.hpp"

unsigned int HalfSizePreview::id = 0;

HalfSizePreview::HalfSizePreview() {
    this->set_process_name("GetHalfSizePreview(" + std::to_string(this->id++) + ")");
}

HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {

    HalfSizePreview::image_vector images;

    this->initImageObjects(images);
    this->loadRawImages(images);
    
    this->deleteImageObjects(images);


}



void HalfSizePreview::initImageObjects(image_vector& images) {

    Json filenames = this->process_data_m->get_array("filenames");
    
    for (int i = 0; i < filenames.get_size(); i++) {

        btrgb::image* im = new btrgb::image(filenames.string_at(i));
        images.push_back(im);

    }
}


void HalfSizePreview::deleteImageObjects(image_vector& images) {

    for( const auto& im : images) {
        delete im;
    }

}


void loadRawImages(image_vector& images) {
    LibRawReader reader(LibRawReader::LOAD_HALF_SIZE);

    for( const auto& im : images) {
        reader.read(im);
    }

}