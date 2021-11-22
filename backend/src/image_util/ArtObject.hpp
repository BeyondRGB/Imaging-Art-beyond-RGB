#ifndef ARTOBJ_H
#define ARTOBJ_H

#include <string>
#include <vector>
#include <unordered_map>

#include "btrgb.hpp"


typedef enum { 
    none,

    cannot_write_tiff,

    image_does_not_exist,
    image_already_exists

} ArtObjStatus;


class ArtObject {

private:

    std::unordered_map<std::string, btrgb::image*> images;

    ArtObjStatus status;

public:

    ArtObject();
    ~ArtObject();

    void newImage(std::string key, std::string filename);

    void setImage(std::string key, btrgb::image* im);
    btrgb::image* getImage(std::string key);
    bool imageExists(std::string key);

    /* To do, add parameter for photometric tiff tag: RGB, or grayscale. */
    void outputImageAsTIFF(std::string key);

    ArtObjStatus exit_status() const {return status;};

};



#endif
