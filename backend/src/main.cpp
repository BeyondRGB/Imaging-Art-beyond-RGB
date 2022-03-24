
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
        1,2,5,608
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

    res.store_string("test", "ahahahahah");

    res.store_double("My_Test_Double", 5.6);

    std::string file_name = "results.btrgb";

    jsoncons::json j = res.jsonafy();
    std::string out;
    j.dump_pretty(out);
    std::cout << "Writing " << file_name << std::endl;
    std::ofstream file;

    file.open(file_name);
    file << out;
    file.close();

    std::string csv_file_name = "results.csv";
    file.open(csv_file_name);
    res.write_results(file);
    file.close();

    
    std::cout << "Reading" << std::endl;
    try{
      jsoncons::json new_json;
      try{
        new_json = Jsonafiable::json_from_file(file_name);
      }catch(std::exception e){
        std::cout << "Cant Read file: " << e.what() << std::endl;
      }
      CalibrationResults res2(new_json);
      res2.write_results(std::cout);
    }catch(std::exception e){
      std::cout << "Error reading" << e.what() << std::endl;
    }



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
