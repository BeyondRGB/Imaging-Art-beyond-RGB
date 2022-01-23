
#include <iostream>
#include "server/request_server.hpp"

//Testing
#include "reference_data/data_manager.hpp"
#include "reference_data/color_patch.hpp"
#include "reference_data/ref_data_defines.hpp"
void testFunc() {
	// Add testing logc here if this is called the server will not be started
    std::string path = REF_DATA_PATH;
    std::string files[4] = {
        "CC_Classic_Reflectance_Data.csv",
        "APT_Reflectance_Data.csv",
        "CCSG_Reflectance_Data.csv",
        "NGT_Reflectance_Data.csv"

    };
    //file_name = "C:\\Users\\ThinkPad41\\Documents\\GitHub\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\test.csv";
    int row = 1;
    int col = 2;
    for (int file = 0; file < 4; file++) {
        std::string file_name = files[file];
        RefData* rd = DataManager::get_instance()->get_ref_data(file_name);
        std::string header = "ValueType";
        std::string y_values = "Y";
        std::string x_values = "X";
        std::string z_values = "Z";

        for (int row = 0; row < rd->get_row_size(); row++) {
            for (int col = 0; col < rd->get_col_size(); col++) {
                std::string comma = ",";
                ColorPatch* cp = rd->get_color_patch(row, col);
                header += comma + cp->get_name();
                y_values += comma + std::to_string(cp->get_y());
                x_values += comma + std::to_string(cp->get_x());
                z_values += comma + std::to_string(cp->get_z());
            }
        }
        std::cout << file_name << std::endl;
        std::cout << header << std::endl;
        std::cout << x_values << std::endl;
        std::cout << y_values << std::endl;
        std::cout << z_values << std::endl;
    }
    //std::cout << cp << std::endl;
   // std::cout << "Y value: " << cp->get_y() << " row: " << row << " col: " << col << std::endl;
    //std::cout << "row: " << cp->get_row() << " col: " << cp->get_col() << std::endl;
    DataManager::get_instance()->shut_down();
}


int main() {
	bool test = true; // Set to true if you want to test something and bypass the server
	if (test) {
		testFunc();
		//std::cout << "Exit" << std::endl;
		exit(1);
	}

    std::cout << "BeyondRBG Backend Started: listening on port 9002\n";
    RequestServer server;
    server.init_server();
    server.start_server();

    //Block till server shuts down
    std::cout << "Exiting.\n";
    
}
