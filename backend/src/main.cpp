
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
#include "image_processing/results/calibration_results.hpp"
#include <opencv2/opencv.hpp>
void testFunc() {
    CalibrationResults res;
    cv::Mat r1 = (cv::Mat_<double>(3,4) << 
        1.0,2.0,3.0,4.0,
        1.0,2.0,3.0,4.0,
        1.0,2.0,5.0,6.0
    );

    cv::Mat r2 = (cv::Mat_<double>(2,3) <<
        1.5,   2.56, 5.0,
        0.345, 0.24, 0.4
    );

    res.set_result_matrix(CalibrationResults::ResultMatrix::DELTA_E, r1);
    res.set_result_matrix(CalibrationResults::ResultMatrix::M_REFL, r2);
    res.set_result_matrix(CalibrationResults::ResultMatrix::M, r1);

    std::string file_name = "results.csv";

    std::cout << "Writing " << file_name << std::endl;
    std::ofstream file;
    file.open(file_name);
    res.write_results(file);
    file.close();

    std::cout << "Reading " << file_name << std::endl;
    CalibrationResults res_in(file_name);
    std::cout << "Showing Contents" << std::endl;
    res_in.write_results(std::cout);



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
