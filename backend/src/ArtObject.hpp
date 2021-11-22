#ifndef ARTOBJ_H
#define ARTOBJ_H

#include <string>
#include <vector>
#include <unordered_map>

#include "btrgb.hpp"

using namespace std;

typedef enum { 
    none,

    cannot_write_tiff,

    image_does_not_exist,
    image_already_exists

} ArtObjStatus;

class ArtObject {

private:

    unordered_map<string, btrgb::image*> images;

    ArtObjStatus status;

public:

    ArtObject(vector<string> input_files);

    void setImage(string name, btrgb::image* im);
    btrgb::image* getImage(string name);
    bool imageExists(string name);

    void outputImageAsTIFF(string name);

    ArtObjStatus exit_status() const {return status;};

};



#endif
