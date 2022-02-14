//
// Defines a class to manage communication between a process and the front end
//

#include "comunication_obj.hpp"

CommunicationObj::CommunicationObj(server* s, websocketpp::connection_hdl hd1, message_ptr msg) {
	server_m = s;
	connectionHandle_m = hd1;
	opcode_m = msg->get_opcode();
}

CommunicationObj::CommunicationObj(const CommunicationObj &other) {
	server_m = other.server_m;
	connectionHandle_m = other.connectionHandle_m;
	opcode_m = other.opcode_m;
}

void CommunicationObj::send_msg(std::string msg) {
	server_m->send(connectionHandle_m, msg, opcode_m);
}

void CommunicationObj::set_id(int newID){
	id = newID;
}

void CommunicationObj::send_info(std::string msg, std::string sender){
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "Info");
	jsoncons::json response_data;
	response_data.insert_or_assign("message", msg);
	response_data.insert_or_assign("sender", sender);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	std::cout<<all_info<<std::endl;
	send_msg(all_info);
}

void CommunicationObj::send_error(std::string msg, std::string sender){
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "Error");
	jsoncons::json response_data;
	response_data.insert_or_assign("message", msg);
	response_data.insert_or_assign("sender", sender);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	std::cout<<all_info<<std::endl;
	send_msg(all_info);
}

void CommunicationObj::send_progress(double val, std::string sender){
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "Progress");
	jsoncons::json response_data;
	response_data.insert_or_assign("value", val);
	response_data.insert_or_assign("sender", sender);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	std::cout<<all_info<<std::endl;
	send_msg(all_info);
}

void CommunicationObj::send_base64(btrgb::Image* image){
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "ImageBase64");
	//Commented out to talk about where we are getting the base 64 and image key name
	jsoncons::json response_data;
/*	response_data.insert_or_assign("dataURL", "data:image/png;base64," + image->getBase64());
	response_data.insert_or_assign("name", image->getName());*/
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	std::cout<<all_info<<std::endl;
	send_msg(all_info);
}

voic CommunicationObj::send_binary(btrgb::Image* image){
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "ImageBinary");
	//Commented out to talk about where we are getting the binary ID, image type and image key name
	jsoncons::json response_data;
/*	response_data.insert_or_assign("id", image->getID());
	response_data.insert_or_assign("type", image->getType());
	response_data.insert_or_assign("name", image->getName());*/
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	std::cout<<all_info<<std::endl;
	send_msg(all_info);
}
