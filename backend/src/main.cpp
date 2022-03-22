
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
#include "image_processing/results/calibration_results.hpp"
#include <opencv2/opencv.hpp>
void testFunc() {
    CalibrationResults res;
    cv::Mat r1 = (cv::Mat_<int>(3,4) << 
        1,2,3,4,
        1,2,3,4,
        1,2,5,60
    );

    cv::Mat r2 = (cv::Mat_<double>(2,3) <<
        1.5,   2.56, 5.0,
        0.345, 0.24, 0.4
    );

    res.store_matrix("DELTA_E", r1);
    res.store_matrix("M_REFL", r2);
    res.store_matrix("M", r1);

    res.store_int("int1", 1);
    res.store_int("int2", 2);

    res.store_double("My_Test_Double", 5.6);

    std::string file_name = "results.csv";

    jsoncons::json j = res.jsonafy();
    std::string out;
    j.dump_pretty(out);

    CalibrationResults res2(j);
    res2.write_results(std::cout);
    // std::cout << out << std::endl;

    // std::cout << "Writing " << file_name << std::endl;
    // std::ofstream file;
    // file.open(file_name);
    // res.write_results(file);
    // file.close();

    // std::cout << "Reading " << file_name << std::endl;
    // CalibrationResults res_in(file_name);
    // try{
    //     // res_in.write_results(std::cout);

    //     std::cout << res_in.get_result_matrix("DELTA_E")<< std::endl;
    //     std::cout << res_in.get_result_matrix("M_REFL")<< std::endl;
    //     std::cout << "int1: " << res_in.get_result_int("int1") << std::endl;
    //     std::cout << "my_Test_double: " << res_in.get_result_double("My_Test_Double") << std::endl;
    // }catch(ResultError e){
    //     std::cout << "Error: " << e.what() << std::endl;
    // }
    // catch(std::exception e){
    //     std::cout << "Error: " << e.what() << std::endl;
    // }
    
    // std::cout << "Showing Contents" << std::endl;
    // res_in.write_results(std::cout);



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
