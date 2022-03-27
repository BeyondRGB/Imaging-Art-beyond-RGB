
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
#include <opencv2/opencv.hpp>
#include "utils/calibration_util.hpp"
void testFunc() {
    cv::Mat test = (cv::Mat_<int>(6,4, CV_16FC1) << 1,2,3,4,
												    2,3,4,5,
													3,4,5,6,
													4,5,6,7,
													5,6,7,8,
													6,7,8,9);

	std::cout << test << std::endl;
	int width = 2;
	int height = 2;

	btrgb::Image *img = btrgb::calibration::camera_sigs_2_image(test, height);
	cv::Mat imgMat = img->getMat();
	// btrgb::calibration::display_matrix(&imgMat, "img");
	std::cout << imgMat << std::endl;
	std::cout << "rows: " << imgMat.rows << " cols: " << imgMat.cols << " Chan: " << imgMat.channels() << std::endl;

	cv::Mat revert = btrgb::calibration::image_2_camera_sigs(img, height, width);
	std::cout << revert << std::endl;

	// std::vector<cv::Mat> channels;
	// for(int row = 0; row < test.rows; row++){
	// 	cv::Rect crop_rect(0,row,width*height,1);
	// 	cv::Mat chan = test(crop_rect);
	// 	std::cout << chan << std::endl;
	// 	channels.push_back(chan);
	// }
	// std::cout << "===================" << std::endl;
	// // std::cout << channels << std::endl;
	// std::cout << "===================" << std::endl;


	// cv::Mat six_chan;
	// cv::merge(channels, six_chan);

	
	// std::cout << six_chan << std::endl;
	// btrgb::calibration::display_matrix(&test, "Test");
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
