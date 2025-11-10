#include <iostream>
#include <stacktrace>
#include <server/process_manager.hpp>
#include <backend_process/ColorManagedImage.hpp>
#include <backend_process/ReportRequest.hpp>
#include <backend_process/SpectralPicker.hpp>
#include <backend_process/SpectralPickerMeasured.hpp>
#include <backend_process/ThumbnailLoader.hpp>

void ProcessManager::process_request(std::string request, std::shared_ptr<CommunicationObj> coms_obj) {
	this->set_coms_obj(coms_obj);
	std::cout << "Received: " << request << std::endl;
	std::cout << "Parsing Request" << std::endl;

	std::string RequestIDKey = key_map[ProcessManager::RequestKey::REQUEST_ID];
	std::string RequestTypeKey = key_map[ProcessManager::RequestKey::REQUEST_TYPE];
	std::string RequestDataKey = key_map[ProcessManager::RequestKey::REQUEST_DATA];
	try {
		// Pars Json
		Json j(request);
		std::string request_key = j.get_string(RequestTypeKey);
		unsigned long request_id = j.get_number(RequestIDKey);
		Json request_data = j.get_obj(RequestDataKey);
		coms_obj->set_id(request_id);

		// Create process
		std::shared_ptr<BackendProcess> process = identify_process(request_key);

		//Start processing thread
		std::thread p_thread(&ProcessManager::start_process, this, process, coms_obj, request_data);
		p_thread.detach();
	}
	catch (ParsingError e) {
		this->report_error("ProcessManager", e.what(), std::stacktrace::current());
	}
}

std::shared_ptr<BackendProcess> ProcessManager::identify_process(std::string key) {
	std::shared_ptr<BackendProcess> process(nullptr);
	std::cout << "RequestType: " << key << std::endl;

	if (key == "Process")
		process = std::shared_ptr<Pipeline>(new Pipeline("Pipeline"));
	
	else if (key == "HalfSizePreview")
		process = std::shared_ptr<HalfSizePreview>(new HalfSizePreview(key));
	
	else if (key == "Thumbnails")
		process = std::shared_ptr<ThumbnailLoader>(new ThumbnailLoader(key));
	
	else if (key == "ColorManagedImage")
		process = std::shared_ptr<ColorManagedImage>(new ColorManagedImage(key));
	
	else if (key == "SpectralPicker")
		process = std::shared_ptr<SpectralPicker>(new SpectralPicker(key));
	
	else if (key == "Reports")
		process = std::shared_ptr<ReportRequest>(new ReportRequest(key));

	else if (key == "SpectralPickerMeasured")
		process = std::shared_ptr<SpectralPickerMeasured>(new SpectralPickerMeasured(key));
	

	return process;
}

void ProcessManager::start_process(std::shared_ptr<BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj, Json request_data) {
	std::cout << "Finalizing Process Initialization" << std::endl;
	if (nullptr == process) {
		this->report_error("ProcessManager", "Unknown RequestType");
		return;
	}
	process->set_coms_obj(coms_obj);
	process->set_process_data(request_data);
	std::cout << "Starting Process" << std::endl;
	process->run();
	std::cout << process->get_process_name() << " complete." << std::endl;
}
