
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
//#include "reference_data/illuminants.hpp"
#include "reference_data/ref_data.hpp"
void testFunc() {
    std::cout << "TestFunc" << std::endl;
    std::cout << "App_Root: " << GlobalsSinglton::get_instance()->app_root() << std::endl;
    IlluminantType illum = Illuminants::D50;
    ObserverType observer = StandardObserver::SO_1931;
    std::string file = "APT_Reflectance_Data.csv";
    RefData data(file, illum, observer);
    data.output_xyz();
}


int main(int argc, char** argv) {
    CMDArgManager::process_args(argc, argv);
	bool test = true; // Set to true if you want to test something and bypass the server
	if (GlobalsSinglton::get_instance()->is_test()) {
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
    delete GlobalsSinglton::get_instance();
    
}
