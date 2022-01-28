#ifndef ARTOBJ_H
#define ARTOBJ_H

#include <string>
#include <vector>
#include <unordered_map>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/ImageWriter.hpp"
#include "ImageUtil/ImageWriter/LibTiffWriter.hpp"
#include "reference_data/ref_data.hpp"

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
        double tLeftX, tLeftY, tRightX, tRightY, bRightX, bRightY, bLeftX, bLeftY;
        int row, col;
        std::unordered_map<std::string, image*> images;
        ImageWriter* tiffWriter;
        RefData* ref_data;

    public:
        ArtObject(std::string ref_file, IlluminantType ilumination, ObserverType observer);
        ~ArtObject();

        void newImage(std::string name, std::string filename);
        void targetInfo(double topLeftX, double topLeftY, double topRightX, double topRightY, double botRightX, double botRightY, double botLeftX, double botLeftY, int rows, int cols);
        void setImage(std::string name, image* im);
        image* getImage(std::string name);
        double getTargetInfo(std::string type);
        int getTargetSize(std::string edge);
        void deleteImage(std::string name);
        bool imageExists(std::string name);
        RefData* get_refrence_data();

        /* To do, add parameter for photometric tiff tag: RGB, or grayscale. */
        void outputImageAsTIFF(std::string name);

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
