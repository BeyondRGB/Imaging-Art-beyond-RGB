#ifndef BACKEND_PROCESS_H
#define BACKEND_PROCESS_H


#include "server/comunication_obj.hpp"
#include "server/communicator.hpp"
#include "utils/json.hpp"

/*
Abstract class representing all BackendProcess's
This class should be inherited by any Class that will be 
requestable process by the front end and run by the Processmanager
Any class that inherits this class is responsible for overriding and 
defining a run method
Any class that inherits this class also inherits the ability to set and use
CommunicationObj's to send msgs to the front end.
*/
class BackendProcess: public Communicator{

public:
	BackendProcess(std::string name);
	virtual ~BackendProcess() {}
	/*
	Pure virtual run function
	Any class inheriting this class is required to override and define this
	*/
	virtual void run() = 0;

	/*
	Accessor method for the process name
	@return the name of the process,
		if no name is set this will return "Undefined Process"
	*/
	std::string get_process_name() { return name; }

	/**
	* Set the process data.
	* Process is any spicific data provided by the frontend with the request
	* Porcess data can include things such as image file nams 
	* @param data: a json object containing any and all process data provided with request
	*	data should be the json obj found in the RequestData feild in original request string
	*/
	void set_process_data(Json data) { process_data_m = std::shared_ptr<Json>(new Json(data)); }

private:
	static unsigned int pid;
	std::string name = "Undefined Process";

protected:
	std::shared_ptr<Json> process_data_m;
};

#endif
