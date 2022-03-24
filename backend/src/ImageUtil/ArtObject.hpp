#ifndef ARTOBJ_H
#define ARTOBJ_H

#include <string>
#include <vector>
#include <unordered_map>

#include "ImageUtil/Image.hpp"
#include "ImageUtil/ImageWriter/ImageWriter.hpp"
#include "reference_data/ref_data.hpp"
#include "ImageUtil/ColorTarget.hpp"
#include "image_processing/results/calibration_results.hpp"

// Macros for identifying images in "images" map
// Example ART(1) will expand to "art1" ART(2) will expand to "art2"
// NOTE: using these will not work with a variable passed in, Only used with hard coded numbers
#define ART(num) "art"#num
#define DARK(num) "dark"#num
#define WHITE(num) "white"#num

/* How to iterate over all images in the ArtObject:
 *
 * key:     std::string
 * im:      btrgb::Image*
 * images:  btrgb::ArtObject*
 *
 *      for(const auto& [key, im] : *images) {
 *          //do stuff here
 *      }
 *
 */



namespace btrgb {

    enum ResultType{
        CALIBRATION,
        GENERAL,
        VERIFICATION
    };

    class ArtObject {

    private:
        TargetData target_data;
        std::unordered_map<std::string, Image*> images;
        RefData* ref_data;
        std::string output_directory;
        CalibrationResults general_res;
        CalibrationResults calibration_res; 
        CalibrationResults verification_res;

    public:
        ArtObject(std::string ref_file, IlluminantType ilumination, ObserverType observer, std::string output_directory);
        ~ArtObject();

        RefData* get_refrence_data();

        void setTargetInfo(TargetData td);
        double getTargetInfo(std::string type);
        int getTargetSize(std::string edge);
        ColorTarget get_target(std::string imageName);

        CalibrationResults *get_results_obj(btrgb::ResultType type);
        std::string get_output_dir();

        void newImage(std::string name, std::string filename);
        void setImage(std::string name, Image* im);
        Image* getImage(std::string name);
        void deleteImage(std::string name);
        bool imageExists(std::string name);

        int imageCount();

        void outputImageAs(enum output_type filetype, std::string name, std::string filename = "");

        /* Iterators over all image entries. */
        std::unordered_map<std::string, Image*>::iterator begin() noexcept {return images.begin();};
        std::unordered_map<std::string, Image*>::iterator end() noexcept {return images.end();};
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
