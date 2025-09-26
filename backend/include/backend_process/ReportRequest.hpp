#ifndef REPORTREQUEST_H
#define REPORTREQUEST_H

#include <iostream>
#include <fstream>
#include <streambuf>
#include <image_util/Image.hpp>
#include <utils/jsonafiable.hpp>
#include <server/communication_obj.hpp>

#include "backend_process.hpp"



class ReportRequest : public BackendProcess {

public:
    ReportRequest(std::string name) : BackendProcess(name) {};
    ~ReportRequest();
	void run() override;
    
};


#endif
