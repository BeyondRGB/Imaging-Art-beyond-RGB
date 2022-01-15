#ifndef REQUEST_SERVER_H
#define REQUEST_SERVER_H

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_STRICT_ 
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "comunication_obj.hpp"
#include "process_manager.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

/*
Class that establishes the server that will listen for requests from the front end
*/
class RequestServer {
	
private:
	server server_m;
	ProcessManager process_manager_m;
	int port_m = 9002; // should this be defined by a config file?
	/*
	Handler that gets called any time a new msg comes in on port_m
	*/
	void msg_handler(server* s, websocketpp::connection_hdl hdl, message_ptr msg);


public:
	RequestServer() {};
	/*
	Initialize the server
		Establish log settings
		Sets Handelers
		Sets port to listen on
	*/
	void init_server();
	/*
	Starts server run loop
	*/
	void start_server();
	void shutdown();


};

#endif //REQUEST_SERVER_H