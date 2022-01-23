
#include <iostream>
#include "server/request_server.hpp"

//Testing
#include "reference_data/data_manager.hpp"
#include "reference_data/color_patch.hpp"
#include "reference_data/ref_data_defines.hpp"
void testFunc() {
	// Add testing logc here if this is called the server will not be started
    std::string path = REF_DATA_PATH;
    std::string file_name = "APT_Reflectance_Data.csv";
    //file_name = "C:\\Users\\ThinkPad41\\Documents\\GitHub\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\test.csv";

    ColorPatch* cp = DataManager::get_instance()->get_ref_data(file_name)->get_color_patch(0,0);
    std::cout << cp->get_y() << std::endl;
    std::cout << "row: " << cp->get_row() << " col: " << cp->get_col() << std::endl;
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
