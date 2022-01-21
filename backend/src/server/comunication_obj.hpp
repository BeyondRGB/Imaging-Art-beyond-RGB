//
// Class for sending messages back to the sender of TCP connection
//

#ifndef COMMUNICATION_OBJ_H
#define COMMUNICATION_OBJ_H

#include <iostream>

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_STRICT_ 

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

/**
* Class used for sending mesages to the front end.
* An instance of the class gets created when a new msg is recieved from the forntedn.
* It can then be passed on to a process so that the process can send mesages as needed
*/
class CommunicationObj {
private:
	server* server_m = NULL;
	websocketpp::connection_hdl connectionHandle_m;
	websocketpp::frame::opcode::value opcode_m;

public:
	CommunicationObj() {};
	CommunicationObj(server* s, websocketpp::connection_hdl hd1, message_ptr msg);
	/**
	* Copy Constructor	
	*/
	CommunicationObj(const CommunicationObj& other);
	
	/**
	* Function for sending a message back to the front end
	* @param msg: the message string to send
	*/
	void send_msg(std::string msg);
};

#endif // COMMUNICATION_OBJ_H