//
// Class for sending messages back to the sender of TCP connection
//

#ifndef COMMUNICATION_OBJ_H
#define COMMUNICATION_OBJ_H

#include <iostream>
#include <jsoncons/json.hpp>

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_STRICT_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "ImageUtil/Image.hpp"

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
	unsigned long id;
	/**
	* Function for sending a message back to the front end
	* @param msg: the message string to send
	*/
	void send_msg(std::string msg);
	void send_bin(std::vector<uchar>& v);

    btrgb::base64_ptr_t createDataURL(enum btrgb::output_type type, std::vector<uchar>* direct_binary);

public:
	CommunicationObj() {};
	CommunicationObj(server* s, websocketpp::connection_hdl hd1, message_ptr msg);
	/**
	* Copy Constructor
	*/
	CommunicationObj(const CommunicationObj& other);

	//void send_msg(std::string msg);
	void set_id(long newID);
	/**
	* Function for sending a Information Message to the front end
	* @param msg: the message being sent to the front end
	* @param sender: what function is sending the message
	*/
	void send_info(std::string msg, std::string sender);
	/**
	* Function for sending a Error Message to the front end
	* @param msg: the message being sent to the front end
	* @param sender: what function is sending the message
	*/
	void send_error(std::string msg, std::string sender);
	/**
	* Function for sending a Progress Update Message to the front end
	* @param val: amount of progress made in a overall step
	* @param sender: what function is sending the message
	*/
	void send_progress(double val, std::string sender);
	/**
	* Function for sending a base64 image to the front end
	* @param image: pointer to the image object of the image being sent
	* @param type: enum to the type of image being sent
	* @param qual: enum for the quality of the image being sent
	*/
	void send_reports(jsoncons::json reports, std::string report_type);

	void send_spectrum(float* data, int size);
	void send_pipeline_components(jsoncons::json compoents_list);

	void send_base64(btrgb::Image* image, enum btrgb::image_quality qual);
	void send_binary(btrgb::Image* image, enum btrgb::image_quality qual);

	void send_base64(
		std::string name,
		std::vector<uchar>* direct_binary,
		enum btrgb::output_type type
	);

	void send_binary(
		std::string name,
		std::vector<uchar>* direct_binary,
		enum btrgb::output_type type
	);

	void send_post_calibration_msg(std::string results_pah);
};

#endif // COMMUNICATION_OBJ_H
