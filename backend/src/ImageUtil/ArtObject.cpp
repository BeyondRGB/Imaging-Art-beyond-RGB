#include "ArtObject.hpp"

namespace btrgb {

    ArtObject::ArtObject() {
        this->tiffWriter = new LibTiffWriter();
    }

    /* 
    * Destructor deletes all btrgb::image structures that were made.
    */
    ArtObject::~ArtObject() {

        /* Delete tiff writer. */
        delete this->tiffWriter;

        /* Delete every image in the map. */
        for( const auto& [name, img] : this->images )
            delete img;
    }


    /* 
    * Creates and maps a new image.
    */
    void ArtObject::newImage(std::string name, std::string filename) {

        if(this->images.contains(name))
            throw ArtObj_ImageAlreadyExists();

        image* im = new image(filename);

        this->images[name] = im;
    }


    /* 
    * Map an image name to an existing image object.
    * If the name is used, throws ArtObj_ImageAlreadyExists.
    */
    void ArtObject::setImage(std::string name, image* im) {

        if (this->images.contains(name))
            throw ArtObj_ImageAlreadyExists();

        this->images[name] = im;
    }


    /* 
    * Returns a pointer to an image object if it is
    * is found in memory, otherwise throws ArtObj_ImageDoesNotExist. 
    */
    image* ArtObject::getImage(std::string name) {

        if(this->images.contains(name))
            return this->images[name];

        throw ArtObj_ImageDoesNotExist();
    }


    /* 
    * Returns a boolean to indicate if an image is stored in memory.
    */
    bool ArtObject::imageExists(std::string name) {
        return this->images.contains(name);
    }


    /* 
    * The key of the image stored in memory to write to disk. 
    * The image filename will be the filename field in the 
    * btrgb::image structure. 
    */
    void ArtObject::outputImageAsTIFF(std::string name) {

        if (! this->images.contains(name))
            throw ArtObj_ImageDoesNotExist();
        
        try {
            this->tiffWriter->write( this->images[name] );
        }
        catch (ImageWritingError const& e) {
            throw ArtObj_ImageDoesNotExist();
        }
        
    }

}