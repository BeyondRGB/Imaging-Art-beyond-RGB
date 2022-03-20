#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "comunication_obj.hpp"
#include <iostream>

/*
Communications class, used for communication with the front end.
A class that inherits from this class can send mesages to the front end using the
coms obj once it has been set.
NOTE: befor a mesage can be sent successfully set_coms_obj must be called
*/
class Communicator {

public:
	/**
	* Send a mesage to the front end
	* @param msg: the mesage to be sent
	*/
	void send_info(std::string msg, std::string reporter);
	/**
	* Set the CommunicationObj to be used for sending messages
	* @param coms_obj: the CommunicationObj to set
	*/
	void set_coms_obj(std::shared_ptr<CommunicationObj> coms_obj);

	/**
	* Report error
	* This will print error to
	*	print error to consol
	*	report error to fontend
	*	TODO log error?
	* @param error: the error msg to report
	*/
	void report_error(std::string reporter, std::string error);
protected:
	std::shared_ptr<CommunicationObj> coms_obj_m;
};

#endif //COMMUNICATOR_H
