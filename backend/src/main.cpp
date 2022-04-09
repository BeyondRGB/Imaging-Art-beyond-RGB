
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
#include "image_processing/results/calibration_results.hpp"
#include "utils/jsonafiable.hpp"
#include "utils/json.hpp"

#include "image_processing/results/general_info_formater.hpp"
#include "image_processing/results/m_color_formater.hpp"
#include "image_processing/results/colorimetry_formater.hpp"
#include "image_processing/results/r_camera_fromater.hpp"

#include "reference_data/ref_data.hpp"

void testFunc() {

	std::string file = "C:\\Users\\ThinkPad41\\Documents\\School\\CurrentCourses\\BeyondRGB\\test_pics\\out\\BTRGB_2022-04-07_08-41-16\\BTRGB_1649335418.btrgb";
	jsoncons::json res_json = Jsonafiable::json_from_file(file);
	Json my_json(res_json);

	std::string file_path = "D:\\GitRepos\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\NGT_Reflectance_Data.csv";
	RefData ref_data(file_path);
	cv::Mat xyz = ref_data.xyz_as_matrix();

	ResultsFormater *formater;
	try{
		Json general_info = my_json.get_obj("GeneralInfo");
		Json calib_json = my_json.get_obj("CalibrationResults");
		Json ver_json = my_json.get_obj("VerificationResults");

		CalibrationResults gen_info(general_info.get_jsoncons());
		CalibrationResults calib_res(calib_json.get_jsoncons());
		CalibrationResults ver_res(ver_json.get_jsoncons());

		calib_res.store_matrix(CM_XYZ_REF, xyz);
		ver_res.store_matrix(CM_XYZ_REF, xyz);
		int row_count = gen_info.get_int(GI_TARGET_ROWS);
		int col_count = gen_info.get_int(GI_TARGET_COLS);
		calib_res.store_int(GI_TARGET_COLS, col_count);
		calib_res.store_int(GI_TARGET_ROWS, row_count);

		formater = new ColorimetryFormater();

		formater->write_format(std::cout, &ver_res, ResultsFormater::ResultObjType::VERIFICATION);

		// gen_info.write_results(std::cout);
		// std::cout << "\n======================================================\n======================================================\n" << std::endl;
		// calib_res.write_results(std::cout);
		// std::cout << "\n======================================================\n======================================================\n" << std::endl;
		// ver_res.write_results(std::cout);
	}catch(ParsingError e){
		std::cout << e.what() << std::endl;
	}catch(std::exception e){
		std::cout << "Other: " << e.what() << std::endl;
	}

	delete formater;

	
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
