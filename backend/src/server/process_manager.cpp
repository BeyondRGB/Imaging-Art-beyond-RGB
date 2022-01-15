

#include "process_manager.hpp"
#include <jsoncons/json.hpp>

#include <iostream>

using namespace jsoncons;

void ProcessManager::process_request(std::string request, std::shared_ptr<CommunicationObj> coms_obj) {
	std::string sample_request= R"({
		"RequestType":"processImg",
		"RequestData":{}})";
	//TODO pars the request once we are actualy sending json
	json j = json::parse(sample_request);
	std::string request_key = j.get(
		key_map[ProcessManager::RequestKey::REQUEST_TYPE]).as<std::string>();
	
	// Create process
	std::shared_ptr<BackendProcess> process = identify_process(request_key);
	
	//Start processing thread
	std::thread p_thread(&ProcessManager::start_process, this, process, coms_obj);
	p_thread.detach();

}

std::shared_ptr<BackendProcess> ProcessManager::identify_process(std::string key) {
	std::shared_ptr<BackendProcess> process(nullptr);
	std::cout << "ProcessKey: " << key << std::endl;
	if (key == "processImg") {
		std::cout << "Creating Pipeline" << std::endl;
		process = std::shared_ptr<Pipeline>(new Pipeline());
	}
	//TODO add other process instantiation here as more get created

	return process;
}

void ProcessManager::start_process(std::shared_ptr<BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj) {
	if (nullptr == process) {
		std::cout << "Failed to start process" << std::endl;
		coms_obj->send_msg("Unknown RequestType");
	}
	process->set_coms_obj(coms_obj);
	process->run();
	std::cout << process->get_process_name() << " complete." << std::endl;
}