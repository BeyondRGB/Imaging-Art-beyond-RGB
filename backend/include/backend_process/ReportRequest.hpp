#ifndef REPORTREQUEST_H
#define REPORTREQUEST_H

#include <fstream>
#include <image_util/Image.hpp>
#include <iostream>
#include <server/communication_obj.hpp>
#include <streambuf>
#include <utils/jsonafiable.hpp>

#include "backend_process.hpp"

class ReportRequest : public BackendProcess {

  public:
    ReportRequest(std::string name) : BackendProcess(name) {};
    ~ReportRequest();
    void run() override;
};

#endif
