//
// Abstract class to act as the interface for all image processing classes
//


#ifndef BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H
#define BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H

#include <iostream>
using namespace std;
#include <memory>
#include <vector>

typedef void (*callback)(string);

class ImgProcessingComponent {
public:
    /**
     * Pure virtual fucntion
     * Must be defined by inheritors of this class to preform the execution of
     * processing for that particular component
     */
    virtual void execute(callback func) = 0;
};


#endif //BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H
