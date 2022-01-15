

#include "process_manager.hpp"
#include <jsoncons/json.hpp>

#include <iostream>

using namespace jsoncons;

void ProcessManager::process_request(std::string request, CommunicationObj coms_obj) {
	std::cout << "Request: " << request << std::endl;
	std::string sample_request= R"({
		"RequestType":"processImg",
		"RequestData":{}})";
	//TODO pars the request once we are actualy sending json
	jsoncons::json j = json::parse(sample_request);
	std::string request_key = j.get(
		key_map[ProcessManager::RequestKey::REQUEST_TYPE]).as<std::string>();

	std::shared_ptr<BackendProcess> process = identify_process(request_key, coms_obj);
	std::cout << "CreatedProcess: " << process << std::endl;
	std::thread p_thread(&ProcessManager::start_process, this, process);
	p_thread.detach();

}

std::shared_ptr<BackendProcess> ProcessManager::identify_process(std::string key, CommunicationObj coms_obj) {
	std::shared_ptr<BackendProcess> process(nullptr);
	std::cout << "ProcessKey: " << key << std::endl;
	if (key == "processImg") {
		std::cout << "Creating Pipeline" << std::endl;
		process = std::shared_ptr<Pipeline>(new Pipeline(coms_obj));
	}
	//TODO add other process instantiation here as more get created

	return process;
}

void ProcessManager::start_process(std::shared_ptr<BackendProcess> process) {
	if (nullptr == process) {
		std::cout << "Failed to start process" << std::endl;
	}
	std::cout << "Starting Process:" << process << std::endl;
	process->run();
	std::cout << process->get_process_name() << " complete." << std::endl;
}