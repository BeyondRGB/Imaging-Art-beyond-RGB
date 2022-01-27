
#include <iostream>
#include "server/request_server.hpp"

//Testing
#include "reference_data/data_manager.hpp"
#include "reference_data/color_patch.hpp"
#include "reference_data/ref_data_defines.hpp"
#include "reference_data/standard_observer.hpp"
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
    Illuminants::IlluminantType illum_type = Illuminants::IlluminantType::D50;
    StandardObserver::ObserverType so_type = StandardObserver::ObserverType::SO_1931;
    for (int file = 0; file < 4; file++) {
        std::string file_name = files[file]; 
        RefData rd(file_name);
        int row = 1;
        int col = 2;
        std::cout << file_name << std::endl;
        ColorPatch* cp = rd.get_color_patch(row, col);
        std::cout << *cp << 
            "{x:" << cp->get_x() <<
            ", y:" << cp->get_y() <<
            ", z:" << cp->get_z() << std::endl;
        std::cout << "RefDataTest" << 
            "{x:" << rd.get_x(row, col) <<
            ", y:" << rd.get_y(row, col) <<
            ", z:" << rd.get_z(row,col) << std::endl << std::endl;
        //rd.output_xyz();
        /*RefData* rd = DataManager::get_instance()->get_ref_data(file_name);
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
        ColorPatch* cp = rd->get_white_patch();
        std::cout << "White Patch," << cp->get_name() << std::endl;
        std::cout << "Y Value, " << cp->get_y() << ",Row," << row << ",col," << col << std::endl << std::endl;*/

    }
    //std::string file_name = files[0];
    //RefData* rd = DataManager::get_instance()->get_ref_data(file_name);
    //ColorPatch* cp = rd->get_white_patch();
    //std::cout << cp->get_name() << std::endl;
    //std::cout << "Y value: " << cp->get_y() << " row: " << row << " col: " << col << std::endl;
    //std::cout << "row: " << cp->get_row() << " col: " << cp->get_col() << std::endl;
    
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
