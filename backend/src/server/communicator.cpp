#include "communicator.hpp"

void Communicator::send_msg(std::string msg) {
	if (nullptr == coms_obj_m) {
		std::cout << "No CommunicationObj set, unable to send msg.";
		return;
	}
	coms_obj_m->send_msg(msg);
}

void Communicator::set_coms_obj(std::shared_ptr<CommunicationObj> coms_obj) {
	coms_obj_m = coms_obj;
}