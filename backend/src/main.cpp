
#include <iostream>
#include "server/request_server.hpp"
#include "utils/logger.hpp"

//Testing Includes: Remove before submiting PR
#include "image_processing/header/ColorManagedCalibrator.h"
#include <opencv2/opencv.hpp>
#include "reference_data/ref_data.hpp"
#include "ImageUtil/ArtObject.hpp"
#define TEST(num) "test"#num
void testFunc() {
    // // std::cout << "Main" << std::endl;
    // // RefData ref_data("NGT_Reflectance_Data.csv");
    // // std::cout << "RefData created outpiting Now" << std::endl;
    // // ref_data.output_xyz();
    // ColorManagedCalibrator c;
    // cv::Mat_<double> m1(3, 2, CV_32FC1);
    // cv::Mat_<double> m2(2, 3, CV_32FC1);

    // float num = 1;
    // for (int row = 0; row < m1.rows; row++) {
    //     for (int col = 0; col < m1.cols; col++) {
    //         m1.at<double>(row, col) = num;
    //         num += 1;
    //     }
    // }
    // std::cout << "M1" << std::endl;
    // c.display_matrix(&m1, "M1");
    // num = 1;
    // for (int row = 0; row < m2.rows; row++) {
    //     for (int col = 0; col < m2.cols; col++) {
    //         m2.at<double>(row, col) = num;
    //         num += 1;
    //     }
    // }
    // std::cout << "M2" << std::endl;
    // c.display_matrix(&m2, "M2");

    // cv::Mat m3 = m1 * m2;// *m2;
    // std::cout << "M1" << std::endl;
    // c.display_matrix(&m1, "M1");
    // std::cout << "M2" << std::endl;
    // c.display_matrix(&m2, "M2");
    // std::cout << "M3" << std::endl;
    // c.display_matrix(&m3, "M3");
    std::cout << ART(1) << std::endl;
    std::cout << DARK(3) << std::endl;
    std::cout << WHITE(2) << std::endl;
    std::cout << TEST(2) << std::endl;
    int i = 5;
    std::cout << TEST(i) << std::endl;

}


int main() {
    std::cout << "Init Logging" << std::endl;
    Logger::init_logging();
    
    std::cout << "Logging initialized" << std::endl;
	bool test = false; // Set to true if you want to test something and bypass the server
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
