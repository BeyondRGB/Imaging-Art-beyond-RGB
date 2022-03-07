
#include <iostream>
#include "server/request_server.hpp"
#include "utils/logger.hpp"

//Testing Includes: Remove before submiting PR
void testFunc() {
    
}


int main() {
    Logger::init_logging();
	bool test = false; // Set to true if you want to test something and bypass the server
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
