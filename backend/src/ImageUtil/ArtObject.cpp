#include "ArtObject.hpp"

namespace btrgb {

    ArtObject::ArtObject(std::string ref_file, IlluminantType ilumination, ObserverType observer) {
        this->ref_data = new RefData(ref_file, ilumination, observer);
    }

    /*
    * Destructor deletes all btrgb::image structures that were made.
    */
    ArtObject::~ArtObject() {

        /* Delete every image in the map. */
        for( const auto& [name, img] : this->images )
            delete img;

        delete this->ref_data;

    }


    /*
    * Creates and maps a new image.
    */
    void ArtObject::newImage(std::string name, std::string filename) {

        if(this->images.contains(name))
            throw ArtObj_ImageAlreadyExists();

        Image* im = new Image(filename);

        this->images[name] = im;
    }

    //Collects the color target information from the request message and saves it in the ArtObject
    void ArtObject::targetInfo(double top, double left, double bot, double right, int rows, int cols) {
        topEdge = top;
        leftEdge = left;
        botEdge = bot;
        rightEdge = right;
        targetRow = rows;
        targetCol = cols;
    }

    //Returns a normalized value of the requested edge of the color target
    double ArtObject::getTargetInfo(std::string type) {
        if (type == "top") {
            return this->topEdge;
        }
        else if (type == "bot") {
            return this->botEdge;
        }
        else if (type == "left") {
            return this->leftEdge;
        }
        else if (type == "right") {
            return this->rightEdge;
        }
        return (double) NULL;
    }

    //Returns the requested dimension of the color target
    int ArtObject::getTargetSize(std::string edge){
        if(edge == "row"){
            return this->targetRow;
        }
        else if(edge == "col"){
            return this->targetCol;
        }
        return (int) NULL;
    }

    /*
    * Map an image name to an existing image object.
    * If the name is used, throws ArtObj_ImageAlreadyExists.
    */
    void ArtObject::setImage(std::string name, Image* im) {

        if (this->images.contains(name))
            throw ArtObj_ImageAlreadyExists();

        this->images[name] = im;
    }


    /*
    * Returns a pointer to an image object if it is
    * is found in memory, otherwise throws ArtObj_ImageDoesNotExist.
    */
    Image* ArtObject::getImage(std::string name) {

        if(this->images.contains(name))
            return this->images[name];

        throw ArtObj_ImageDoesNotExist();
    }


    void ArtObject::deleteImage(std::string name) {
        if( ! this->images.contains(name) )
            throw ArtObj_ImageDoesNotExist();
            
        Image* im = this->images[name];
        delete im;
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
     */
    void ArtObject::outputImageAs(enum output_type filetype, std::string name, std::string filename) {
        
        if (! this->images.contains(name))
            throw ArtObj_ImageDoesNotExist();

        try {
            if(filename != "")
                ImageWriterStrategy(filetype).write( this->images[name], filename );
            else
                ImageWriterStrategy(filetype).write( this->images[name] );

        }
        catch (ImageWritingError const& e) {
            throw ArtObj_FailedToWriteImage();
        }
        catch (ImageNotInitialized const& e) {
            throw;
        }
    }


    RefData* ArtObject::get_refrence_data() {
        return this->ref_data;
    }

}
