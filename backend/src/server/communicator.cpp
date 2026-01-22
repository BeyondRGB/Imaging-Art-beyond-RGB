#include <cpptrace/cpptrace.hpp>
#include <server/communicator.hpp>

void Communicator::send_info(std::string msg, std::string reporter) {
    if (nullptr == coms_obj_m) {
        std::cout << "No CommunicationObj set, unable to send msg.";
        return;
    }
    coms_obj_m->send_info(msg, reporter);
}

void Communicator::set_coms_obj(std::shared_ptr<CommunicationObj> coms_obj) {
    coms_obj_m = coms_obj;
}

void Communicator::report_error(std::string reporter, std::string error,
                                cpptrace::stacktrace trace) {
    this->coms_obj_m->send_error(error, reporter, trace);
    // TODO report to log when loggin gets implemented
}
