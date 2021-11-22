#include "image_util/write_tiff.hpp"

#include "ArtObject.hpp"

ArtObject::ArtObject(vector<string> input_files) {
    
    /* to do, create image structures. */

    this->status = none;
}

void ArtObject::setImage(string name, btrgb::image* im) {

    if(this->images.contains(name)) {
        this->status = image_already_exists;
        return;
    }

    this->status = none;
    this->images[name] = im;
}

btrgb::image* ArtObject::getImage(string name) {

    if(this->images.contains(name)) {
        this->status = none;
        return this->images[name];
    }

    this->status = image_does_not_exist;
    return NULL;
}

bool ArtObject::imageExists(string name) {
    this->status = none;
    return this->images.contains(name);
}

void ArtObject::outputImageAsTIFF(string name) {

    if(this->images.contains(name)) {
        this->status = write()
        return;
    }

    this->status = image_does_not_exist;
    return NULL;

}