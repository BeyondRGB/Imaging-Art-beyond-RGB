#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <thread>

#include <iostream>
#include "comunication_obj.hpp"
#include "backend_process/backend_process.hpp"
#include "backend_process/pipeline.hpp"

/*
Class that magages parsing requests, and spinning up processing threads
When a request comes in the request string is passed off to this class.
It is converted to a Json object (expected format as follows)
	{  "RequestType": <request identifying string>,
		"RequestData": <json object> }
The RequestType is used to identify what process to start.
Once the process is identified and created a new thread is started to run the process
*/
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
	/*
	Process request string and start matching process thread
	@param request: the request string sent from the front end
	@param coms_obj: the CommunicationObj needed to send messages back to frontedn
	*/
	void process_request(std::string request, std::shared_ptr<CommunicationObj> coms_obj);

private:
	/*
	Identifys and creates the requested process.
	@param key: the key identifying what process to create, 
		this should be what is found in the RequestType feild
	@return the BackendProcess to run, 
		if the request string did not conain a valid Request Type this will return a nullptr
	*/
	std::shared_ptr<BackendProcess> identify_process(std::string key);
	/*
	Sets the CommunicationObj and starts the process.
	This function is called from the thread that will be running the process
	@param process: the BackendProcess to run
	@param coms_obj: the CommunicationObj to be used for the process to communicate witht the frontend
	*/
	void start_process(std::shared_ptr <BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj);



};

#endif // PROCESS_MANAGER_H