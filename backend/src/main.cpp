
#include <iostream>
#include "server/request_server.hpp"

//Testing
#include "reference_data/data_manager.hpp"
void testFunc() {
	// Add testing logc here if this is called the server will not be started

    std::string file_name = "C:\\Users\\ThinkPad41\\Documents\\GitHub\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\APT_Reflectance_Data.csv";
    //file_name = "C:\\Users\\ThinkPad41\\Documents\\GitHub\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\test.csv";

    DataManager::get_instance()->get_ref_data(file_name);
    DataManager::get_instance()->shut_down();
}


int main() {
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
