#ifndef ARTOBJ_H
#define ARTOBJ_H

#include <string>
#include <vector>
#include <unordered_map>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/ImageWriterStrategy.hpp"

/* How to iterate over all images in the ArtObject:
 *
 * key:     std::string
 * im:      btrgb::image*
 * images:  btrgb::ArtObject*
 *
 *      for(const auto& [key, im] : *images) {
 *          //do stuff here
 *      }
 * 
 */

namespace btrgb {

    class ArtObject {

    private:
        std::unordered_map<std::string, image*> images;

    public:

        ArtObject();
        ~ArtObject();

        void newImage(std::string name, std::string filename);

        void setImage(std::string name, image* im);
        image* getImage(std::string name);
        void deleteImage(std::string name);
        bool imageExists(std::string name);

        void outputImageAs(enum output_type filetype, std::string name, std::string filename);
        std::string getMimeBase64(std::string name);

        /* Iterators over all image entries. */
        std::unordered_map<std::string, image*>::iterator begin() noexcept {return images.begin();};
        std::unordered_map<std::string, image*>::iterator end() noexcept {return images.end();};
    };

    class ArtObjectError : public std::exception {};
    
    class ArtObj_ImageAlreadyExists : public ArtObjectError {
        public:
        virtual char const * what() const noexcept { return "ArtObject Error: An image with that name already exists."; }
    };

    class ArtObj_ImageDoesNotExist : public ArtObjectError {
        public:
        virtual char const * what() const noexcept { return "ArtObject Error: That image does not exist."; }
    };

    class ArtObj_FailedToWriteImage : public ArtObjectError {
        public:
        virtual char const * what() const noexcept { return "ArtObject Error: Failed to write image."; }
    };

}



#endif
