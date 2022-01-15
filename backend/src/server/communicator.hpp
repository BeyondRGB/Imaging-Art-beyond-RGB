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
	/*
	Send a mesage to the front end
	@param: the mesage to be sent
	*/
	void send_msg(std::string msg);
	/*
	Set the CommunicationObj to be used for sending messages
	*/
	void set_coms_obj(std::shared_ptr<CommunicationObj> coms_obj);

private:
	std::shared_ptr<CommunicationObj> coms_obj_m;
};

#endif //COMMUNICATOR_H
