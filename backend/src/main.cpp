
#include <iostream>
#include "server/request_server.hpp"
#include "spdlog/spdlog.h"
#include "utils/logger.hpp"


void testOther() {
    LOG_ERROR("FromOther");
    LOG_INFO("Info from test Other");
}

//Testing Includes: Remove before submiting PR
void testFunc() {
    
    LOG_ERROR("Test 1 2 3");
    LOG_INFO("From testFunc");
    LOG_ERROR("Test 456");
    testOther();
    LOG_ERROR("Test 789");
}


int main() {
    Logger::init_logging();
	bool test = true; // Set to true if you want to test something and bypass the server
	if (test) {
		testFunc();
		std::cout << "Exit" << std::endl;
		exit(1);
	}

    std::cout << "BeyondRBG Backend Started: listening on port 9002\n";
    RequestServer server;
    server.init_server();
    server.start_server();

    //Block till server shuts down
    std::cout << "Exiting.\n";
    
}
