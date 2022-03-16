
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
#include "reference_data/illuminants.hpp"
#include "reference_data/ref_data.hpp"
#include "reference_data/standard_observer.hpp"
#include "utils/calibration_util.hpp"
#include "utils/time_tracker.hpp"
#include <chrono>
#include <thread>


void testFunc() {
    // ObserverType so = StandardObserver::ObserverType::SO_1931;
    // IlluminantType illum = Illuminants::IlluminantType::D50;
    // std::string file = "APT_Reflectance_Data.csv";
    // RefData ref_data(file, illum, so);
    // cv::Mat m = ref_data.as_matrix();
    // btrgb::calibration::display_matrix(&m, "RefData");

    TimeTracker tracker;
    tracker.start_timeing();
    sleep_for(seconds(30));
    tracker.end_timeing();
    

    tracker.elapsed_time_min();
    tracker.elapsed_time_sec();
    tracker.elapsed_time_ms();
    
    
    // cv::Mat m = (cv::Mat_<double>(3, 5) << 
    //                 3 ,  7, 1,  6,  2,
    //                 10, 33, 5, 42, 14,
    //                  -1,  2, 3,  4,  5

    //                 );

    // btrgb::calibration::display_matrix(&m, "TestMat");
    // int row = 2;
    // double min = btrgb::calibration::row_min(m, row);
    // double max = btrgb::calibration::row_max(m, row);
    // std::cout << "\nMin: " << min << " Max: " << max << std::endl;
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
