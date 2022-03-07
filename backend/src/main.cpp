
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
#include "reference_data/illuminants.hpp"
#include "reference_data/ref_data.hpp"
#include "reference_data/standard_observer.hpp"
#include "utils/calibration_util.hpp"
void testFunc() {
    ObserverType so = StandardObserver::ObserverType::SO_1931;
    IlluminantType illum = Illuminants::IlluminantType::D50;
    std::string file = "APT_Reflectance_Data.csv";
    RefData ref_data(file, illum, so);
    cv::Mat m = ref_data.as_matrix();
    btrgb::calibration::display_matrix(&m, "RefData");

}


int main(int argc, char** argv) {
    std::cout << argv << std::endl;
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
