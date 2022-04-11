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
#include "server/comunication_obj.hpp"
#include <jsoncons/json_reader.hpp>

class ImgProcessingComponent {
public:
    ImgProcessingComponent(std::string name){ this->name = name; }
    virtual ~ImgProcessingComponent() {}
    /**
     * Pure virtual fucntion
     * Must be defined by inheritors of this class to preform the execution of
     * processing for that particular component
     */
    virtual void execute(CommunicationObj* comms, btrgb::ArtObject* images) = 0;
    virtual jsoncons::json get_component_list() = 0;

    std::string get_name(){ return this->name; }

	class error : public std::runtime_error {
		private:
			std::string sender;
        public:
			error(std::string msg, std::string sender) 
				: std::runtime_error(msg) { this->sender = sender; }
			const std::string who() const noexcept { return this->sender; }
	};

private:
    std::string name = "Undefined Component";
};


#endif //BEYOND_RGB_BACKEND_IMGPROCESSINGCOMPONENT_H
