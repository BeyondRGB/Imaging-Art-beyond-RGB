#include "write_tiff.hpp"

#include "ArtObject.hpp"

ArtObject::ArtObject() {
    this->status = none;
}

/* 
 * Destructor deletes all btrgb::image structures that were made.
 */
ArtObject::~ArtObject() {
    for( const auto& [key, value] : this->images ) {
        delete value;
    }
}


/* 
 * Creates and maps a new btrgb::image structure
 */
void ArtObject::newImage(std::string key, std::string filename) {

    if(this->images.contains(key)) {
        this->status = image_already_exists;
        return;
    }

    btrgb::image* im;
    im = new btrgb::image;
    im->filename = filename;
    im->bitmap = 0;
    im->channels = 0;
    im->width = 0;
    im->height = 0;

    this->status = none;
    this->images[key] = im;
}


/* 
 * Map an image key to a btrgb::image struct pointer.
 */
void ArtObject::setImage(std::string key, btrgb::image* im) {

    if(this->images.contains(key)) {
        this->status = image_already_exists;
        return;
    }

    this->status = none;
    this->images[key] = im;
}


/* 
 * Returns a pointer to a btrgb::image structure if the image
 * is found in memory, otherwise NULL is returned. 
 * Possible statuses: "none" on success, "image_does_not_exist". 
 */
btrgb::image* ArtObject::getImage(std::string key) {

    if(this->images.contains(key)) {
        this->status = none;
        return this->images[key];
    }

    this->status = image_does_not_exist;
    return NULL;
}


/* 
 * Returns a boolean to indicate if an image is stored in memory.
 */
bool ArtObject::imageExists(std::string key) {
    this->status = none;
    return this->images.contains(key);
}


/* 
 * The key of the image stored in memory to write to disk. 
 * The image filename will be the filename field in the 
 * btrgb::image structure. 
 */
void ArtObject::outputImageAsTIFF(std::string key) {

    if(this->images.contains(key)) {
        tiff_util::status s;

        s = tiff_util::write(this->images[key]);
        switch(s) {

        case tiff_util::none:
            this->status = none;
            break;

        case tiff_util::cannot_open_for_writing:
            this->status = cannot_write_tiff;
            break;

        case tiff_util::failed_to_write_strip:
            this->status = cannot_write_tiff;
            break;

        }

        return;
    }

    this->status = image_does_not_exist;
}