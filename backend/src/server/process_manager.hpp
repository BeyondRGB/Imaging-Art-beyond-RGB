#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <thread>

#include <iostream>
#include "comunication_obj.hpp"

class ProcessManager {

public:
	ProcessManager() {};
	void process_request(std::string request, CommunicationObj coms_obj);



};

#endif // PROCESS_MANAGER_H