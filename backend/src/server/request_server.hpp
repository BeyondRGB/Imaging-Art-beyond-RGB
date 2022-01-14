#ifndef REQUEST_SERVER_H
#define REQUEST_SERVER_H

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_STRICT_ 

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

//#include <chrono>
//#include <thread>
//
//#include <iostream>
//
//#include "image_processing/pipeline.h"
#include "comunication_obj.hpp"
#include "process_manager.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

class RequestServer {
	
private:
	server server_m;
	ProcessManager process_manager_m;
	int port_m = 9002; // should this be defined by a config file?
	void msg_handler(server* s, websocketpp::connection_hdl hdl, message_ptr msg);


public:
	RequestServer() {};
	void init_server();
	void start_server();
	void shutdown();


};

#endif //REQUEST_SERVER_H