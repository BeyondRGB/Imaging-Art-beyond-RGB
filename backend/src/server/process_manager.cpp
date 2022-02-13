

#include "process_manager.hpp"

#include <iostream>

void ProcessManager::process_request(std::string request, std::shared_ptr<CommunicationObj> coms_obj) {
	this->set_coms_obj(coms_obj);
	this->send_info( this->name_m, "Received: " + request);
	std::cout << "Received: " << request << std::endl;
	std::cout << "Parsing Request" << std::endl;

	std::string RequestIDKey = key_map[ProcessManager::RequestKey::REQUEST_ID];
	std::string RequestTypeKey = key_map[ProcessManager::RequestKey::REQUEST_TYPE];
	std::string RequestDataKey = key_map[ProcessManager::RequestKey::REQUEST_DATA];
	try {
		// Pars Json
		Json j(request);
		std::string request_key = j.get_string(RequestTypeKey);
		int request_id = j.get_number(RequestIDKey);
		Json request_data = j.get_obj(RequestDataKey);
		coms_obj->set_id(request_id);

		// Create process
		std::shared_ptr<BackendProcess> process = identify_process(request_key);

		//Start processing thread
		std::thread p_thread(&ProcessManager::start_process, this, process, coms_obj, request_data);
		p_thread.detach();
	}
	catch (ParsingError e) {
		this->report_error("ProcessManager", e.what());
	}
}

std::shared_ptr<BackendProcess> ProcessManager::identify_process(std::string key) {
	std::shared_ptr<BackendProcess> process(nullptr);
	std::cout << "ProcessKey: " << key << std::endl;
	if (key == "processImg") {
		std::cout << "Creating Pipeline" << std::endl;
		process = std::shared_ptr<Pipeline>(new Pipeline());
	}
	else if (key == "HalfSizePreview") {
		process = std::shared_ptr<HalfSizePreview>(new HalfSizePreview());
	}
	//TODO add other process instantiation here as more get created

	return process;
}

void ProcessManager::start_process(std::shared_ptr<BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj, Json request_data) {
	if (nullptr == process) {
		this->report_error("ProcessManager", "Unknown RequestType");
	}
	process->set_coms_obj(coms_obj);
	process->set_process_data(request_data);
	process->run();
	std::cout << process->get_process_name() << " complete." << std::endl;
}
