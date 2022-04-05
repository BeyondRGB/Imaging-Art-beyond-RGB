#include <regex>
#include "backend_process/ReportRequest.hpp"

unsigned int ReportRequest::id = 0;

ReportRequest::ReportRequest() {
    this->set_process_name("Report(" + std::to_string(ReportRequest::id) + ")");
    ReportRequest::id += 1;
}

ReportRequest::~ReportRequest() {}

void ReportRequest::run() {
    std::string filename = this->process_data_m->get_string("name");
    jsoncons::json convert_json = Jsonafiable::json_from_file(filename);
    Json all_json(convert_json);
    Json report_data = all_json.get_obj("CalibrationResults");
    Json verifcation_data = all_json.get_obj("VerificationResults");
    //Need to add a way to make/adjust current CalibrationResults Obj, don't have access to art obj so can't use that one currently
    this->coms_obj_m->send_reports(report_data.get_jsoncons(), "Calibration");
    this->coms_obj_m->send_reports(verifcation_data.get_jsoncons(), "Verification");
}
