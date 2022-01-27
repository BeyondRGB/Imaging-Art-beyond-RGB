#include "ArtObject.hpp"

namespace btrgb {

    ArtObject::ArtObject(std::string ref_file, IlluminantType ilumination, ObserverType observer) {
        this->tiffWriter = new LibTiffWriter();
        this->ref_data = new RefData(ref_file, ilumination, observer);
        this->ref_data->output_xyz();
    }

    /* 
    * Destructor deletes all btrgb::image structures that were made.
    */
    ArtObject::~ArtObject() {

        /* Delete tiff writer. */
        delete this->tiffWriter;
        delete this->ref_data;

        /* Delete every image in the map.
         * This should be done automatically by the unordered_map destructor:
         *
         * https://www.cplusplus.com/reference/unordered_map/unordered_map/~unordered_map/
         * ~unordered_map();
         * Destroy unordered map
         * Destructs the container object. This calls each of the contained element's destructors, 
        *  and dealocates all the storage capacity allocated by the unordered_map container.
        */
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


    void ArtObject::deleteImage(std::string name) {
        if( ! this->images.contains(name) )
            throw ArtObj_ImageDoesNotExist();

        this->images.erase(name);
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
            throw ArtObj_FailedToWriteImage();
        }
        catch (BitmapNotInitialized const& e) {
            throw;
        }
        
    }

}