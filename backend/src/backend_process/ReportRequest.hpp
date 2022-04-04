#ifndef REPORTREQUEST_H
#define REPORTREQUEST_H

#include <iostream>
#include <fstream>
#include <streambuf>
#include "ImageUtil/Image.hpp"
#include "utils/json.hpp"
#include "server/comunication_obj.hpp"

#include "backend_process.hpp"



class ReportRequest : public BackendProcess {

public:
    ReportRequest();
    ~ReportRequest();
	void run() override;

private:
	static unsigned int id;

};


#endif
