#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <thread>

#include <iostream>
#include "comunication_obj.hpp"
#include "backend_process/backend_process.hpp"
#include "backend_process/pipeline.h"

class ProcessManager {

	enum RequestKey {
		REQUEST_TYPE,
		REQUEST_DATA
	};
	const std::string key_map[2] = {
		"RequestType",
		"RequestData"
	};


public:
	ProcessManager() {};
	void process_request(std::string request, std::shared_ptr<CommunicationObj> coms_obj);

private:
	std::shared_ptr<BackendProcess> identify_process(std::string key);
	void start_process(std::shared_ptr <BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj);



};

#endif // PROCESS_MANAGER_H