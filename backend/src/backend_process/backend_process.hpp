#ifndef BACKEND_PROCESS_H
#define BACKEND_PROCESS_H


#include "server/comunication_obj.hpp"
#include "server/communicator.hpp"

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
	std::string get_process_name() { return m_name; }
	
	/*
	Sets the process name
	@param name: the string to set the name to
	*/
	void set_process_name(std::string name) { m_name = name; }
private:
	std::string m_name = "Undefined Process";
};

#endif // !BACKEND_PROCESS_H
