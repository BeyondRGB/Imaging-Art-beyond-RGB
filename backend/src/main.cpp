
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR

#include "reference_data/ref_data.hpp"
void testFunc() {
	IlluminantType illum =  IlluminantType::D50;
	ObserverType observer = ObserverType::SO_1931;
	std::string path = "C:\\Users\\ThinkPad41\\Documents\\School\\CurrentCourses\\BeyondRGB\\test_pics\\";
	std::string file = "Test_Data.csv";
	std::string adp = "APT_Reflectance_Data.csv";
	try{
		RefData ref_data(path+file, illum, observer);
		int col = ref_data.get_col_count()-1;
		int row = ref_data.get_row_count()-1;
		std::cout << "Row_c: " << row << " col_c: " << col << std::endl;
		ColorPatch *cp = ref_data.get_color_patch(row,col);
		std::cout << cp->get_name() << std::endl;
		std::cout << cp->get_ref_by_index(0) << std::endl;
	}catch(RefData_FailedToRead e){
		std::cout << "Error: " << e.what() << std:: endl;
	}catch(RefData_ParssingError e){
		std::cout << "Error: " << e.what() << std:: endl;
	}catch(std::exception e){
		std::cout << "Error: " << e.what() << std:: endl;
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

	std::cout << "BeyondRBG Backend Started: listening on port " << GlobalsSinglton::get_instance()->get_port() << std::endl;
	RequestServer server;
	server.init_server();
	server.start_server();

  //Block till server shuts down
  std::cout << "Exiting.\n";
  delete GlobalsSinglton::get_instance();


}
