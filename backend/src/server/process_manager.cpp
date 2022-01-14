

#include "process_manager.hpp"
#include <jsoncons/json.hpp>

#include <iostream>

using namespace jsoncons;

void ProcessManager::process_request(std::string request, CommunicationObj coms_obj) {
	std::cout << "Request: " << request << std::endl;
	//TODO add requst parsing and logic for starting up spicific processing thread
	std::string sample_request= R"({"RequestType":"processing from json","RequestData":{}})";
	jsoncons::json j = json::parse(sample_request);
	std::string request_string = j.get(key_map[ProcessManager::RequestKey::REQUEST_TYPE]).as<std::string>();

#ifdef DEBUG
	std::cout << "Debug flag is set" << std::endl;
#endif
	std::cout << "AAAAAAAAAAAAa" << std::endl;

}