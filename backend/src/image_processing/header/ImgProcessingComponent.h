//
// Abstract class to act as the interface for all image processing classes
//


#ifndef BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H
#define BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H

#include <iostream>
#include <memory>
#include <vector>
#include <functional>



#include <chrono>
#include <thread>
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

#include "ImageUtil/ArtObject.hpp"

//typedef void (*CallBackFunction)(string);
typedef std::function<void(std::string)> CallBackFunction;
class ImgProcessingComponent {
public:
    /**
     * Pure virtual fucntion
     * Must be defined by inheritors of this class to preform the execution of
     * processing for that particular component
     */
    virtual void execute(CallBackFunction func, btrgb::ArtObject* images) = 0;
};


#endif //BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H
