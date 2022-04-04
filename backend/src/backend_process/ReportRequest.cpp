#include <regex>
#include "backend_process/ReportRequest.hpp"

unsigned int ReportRequest::id = 0;

ReportRequest::ReportRequest() {
    this->set_process_name("GetReport(" + std::to_string(HalfSizePreview::id) + ")");
    ReportRequest::id += 1;
}

ReportRequest::~ReportRequest() {}
