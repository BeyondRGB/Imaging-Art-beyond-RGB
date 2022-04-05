#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <thread>

#include <iostream>
#include "comunication_obj.hpp"
#include "communicator.hpp"
#include "backend_process/backend_process.hpp"
#include "backend_process/pipeline.hpp"
#include "backend_process/ColorManagedImage.hpp"
#include "backend_process/HalfSizePreview.hpp"
#include "backend_process/ThumbnailLoader.hpp"
#include "utils/json.hpp"

/*
Class that magages parsing requests, and spinning up processing threads
When a request comes in the request string is passed off to this class.
It is converted to a Json object (expected format as follows)
	{  "RequestType": <request identifying string>,
		"RequestData": <json object> }
The RequestType is used to identify what process to start.
Once the process is identified and created a new thread is started to run the process
*/
class ProcessManager : public Communicator {

	enum RequestKey {
		REQUEST_TYPE,
		REQUEST_DATA,
		REQUEST_ID
	};
	/**
	* Maps an enum value to a string
	*/
	const std::string key_map[3] = {
		"RequestType",
		"RequestData",
		"RequestID"
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
	std::string name_m = "ProcessManager";
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
	void start_process(std::shared_ptr <BackendProcess> process, std::shared_ptr<CommunicationObj> coms_obj, Json request_data);

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
			},
			"RefData":{
				"RefData":"NGT_Reflectance_Data.csv",
				"StandardObserver":1931,
				"Illuminants":"D50"
			}

		}
	})";


};

#endif // PROCESS_MANAGER_H
