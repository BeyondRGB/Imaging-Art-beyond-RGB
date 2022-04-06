//
// Defines a class to manage communication between a process and the front end
//

#include <cppcodec/base64_rfc4648.hpp>
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

void CommunicationObj::send_bin(std::vector<uchar>& v){
	const void* binToSend = (void*)v.data();
	//Need to find out how to send bin without this send, since it needs a string for what it is sending
	server_m->send(connectionHandle_m, binToSend, v.size(), websocketpp::frame::opcode::binary);
}

void CommunicationObj::set_id(long newID){
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
	//std::cout<<all_info<<std::endl;
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
	//std::cout<<all_info<<std::endl;
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
	//std::cout<<all_info<<std::endl;
	send_msg(all_info);
}

void CommunicationObj::send_base64(btrgb::Image* image, enum btrgb::image_quality qual){
	btrgb::binary_ptr_t bin = image->getEncodedPNG(qual);
	this->send_base64(image->getName(), bin.get(), btrgb::PNG);
}

void CommunicationObj::send_binary(btrgb::Image* image, enum btrgb::image_quality qual){
	btrgb::binary_ptr_t bin = image->getEncodedPNG(qual);
	this->send_binary(image->getName(), bin.get(), btrgb::PNG);
}

void CommunicationObj::send_base64(
	std::string name,
	std::vector<uchar>* direct_binary,
	enum btrgb::output_type type
) {

	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "ImageBase64");
	jsoncons::json response_data;
	btrgb::base64_ptr_t b64 = this->createDataURL(type, direct_binary);
	response_data.insert_or_assign("dataURL", *b64);
	response_data.insert_or_assign("name", name);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	send_msg(all_info);

};

void CommunicationObj::send_binary(
	std::string name,
	std::vector<uchar>* direct_binary,
	enum btrgb::output_type type
) {

	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "ImageBinary");
	jsoncons::json response_data;
	switch(type) {
			case btrgb::PNG: response_data.insert_or_assign("type", "image/png"); break;
			case btrgb::JPEG: response_data.insert_or_assign("type", "image/jpeg"); break;
			default: throw std::logic_error("[CommunicationObj::send_binary] Invalid image type. ");
	}
	response_data.insert_or_assign("name", name);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	send_msg(all_info);

	/* Temporarily add binID on and send. */
	send_bin(*direct_binary);

	//this->binID++;
}

void CommunicationObj::send_reports(jsoncons::json reports, std::string report_type) {
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "Report");
	jsoncons::json response_data;
	response_data.insert_or_assign("reportType", report_type);
	response_data.insert_or_assign("reports", reports);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	send_msg(all_info);
}

void CommunicationObj::send_spectrum(float* data, int size) {
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "SpectralPicker");
	jsoncons::json response_data;
	response_data["size"] = size;
	response_data["spectrum"] = jsoncons::json::make_array(size);
	for( int i=0; i<size; i++)
		response_data["spectrum"][i] = data[i];
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	send_msg(all_info);
}


btrgb::base64_ptr_t CommunicationObj::createDataURL(enum btrgb::output_type type, std::vector<uchar>* direct_binary) {

	std::string img_type;
	switch(type) {
		/* Supported */
		case btrgb::PNG: img_type = "png"; break;
		case btrgb::JPEG: img_type = "jpeg"; break;
		/* Unsupported */
		case btrgb::TIFF:
		default: throw std::logic_error("[CommunicationObj::createDataURL] Invalid image type. ");
	}

	btrgb::base64_ptr_t result_base64(new std::string(
		"data:image/" + img_type + ";base64," + cppcodec::base64_rfc4648::encode(*direct_binary)
	));

	return result_base64;

}

void CommunicationObj::send_post_calibration_msg(std::string results_path){
	jsoncons::json info_body;
	info_body.insert_or_assign("RequestID", id);
	info_body.insert_or_assign("ResponseType", "CalibrationComplete");
	jsoncons::json response_data;
	response_data.insert_or_assign("path", results_path);
	info_body.insert_or_assign("ResponseData", response_data);
	std::string all_info;
	info_body.dump(all_info);
	send_msg(all_info);
}
