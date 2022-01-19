

#include "process_manager.hpp"

#include <iostream>

void ProcessManager::process_request(std::string request, std::shared_ptr<CommunicationObj> coms_obj) {
	this->set_coms_obj(coms_obj);
	std::string sample_request = R"({
		"RequestType":"processImg",
		"RequestData":{
			"Images":[
				{
					"Art":"art1.png",
					"White":"white1.png",
					"Dark":"dark1.png"
				},
				{
					"Art":"art2.png",
					"White":"white2.png",
					"Dark":"dark2.png"
				}
			],
			"TargetLocation":{
				"TopLeft":{"x": 0, "y":0},
				"TopRight": {"x":50, "y":0},
				"BottomRight": {"x":50, "y":50},
				"BottomLeft": {"x":0, "y":50},
				"NumRows": 10,
				"NumCols":10
			}	
			
		}
	})";
	std::cout << "Parsing Request" << std::endl;
//TODO pars the request once we are actualy sending json
	request = sample_request;

	std::string RequestTypeKey = key_map[ProcessManager::RequestKey::REQUEST_TYPE];
	std::string RequestDataKey = key_map[ProcessManager::RequestKey::REQUEST_DATA];
	try {
		Json j(request);
		std::string request_key = j.get_string(RequestTypeKey);
		Json request_data = j.get_obj(RequestDataKey);
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
	//TODO add other process instantiation here as more get created

	return process;
}

void ProcessManager::start_process(std::shared_ptr<BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj, Json request_data) {
	if (nullptr == process) {
		std::cout << "Failed to start process" << std::endl;
		coms_obj->send_msg("Unknown RequestType");
	}
	process->set_coms_obj(coms_obj);
	process->run();
	std::cout << process->get_process_name() << " complete." << std::endl;
}

//bool ProcessManager::validate_request(json j) {
//	bool result = true; // assume valid
//	std::string RequestTypeKey = key_map[ProcessManager::RequestKey::REQUEST_TYPE];
//	bool request_found = j.has_key(RequestTypeKey);
//	bool request_is_string = j.get(RequestTypeKey).is_string();
//	if (!request_found || !request_is_string) {
//		// RequestType does not exist or value is not a string
//		result = false;
//		report_error(name_m, "RequestType string not found.");
//	}
//	std::string RequestDataKey = key_map[ProcessManager::RequestKey::REQUEST_DATA];
//	bool data_found = j.has_key(RequestDataKey);
//	bool data_is_obj = j.get(RequestDataKey).is_object();
//	if (!data_found || !data_is_obj) {
//		// RequestData does not exist or value is not a json obj
//		result = false;
//		report_error(name_m, "RequestData not found.");
//	}
//	return result;
//}

//std::string ProcessManager::extract_request_str(json j) {
//	std::string key = key_map[ProcessManager::RequestKey::REQUEST_TYPE];
//	return j.get(key).as<std::string>();
//}

