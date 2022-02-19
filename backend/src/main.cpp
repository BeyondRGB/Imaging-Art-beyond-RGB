
#include <iostream>
#include "server/request_server.hpp"

//Testing Includes: Remove before submiting PR
#include "image_processing/header/ColorManagedCalibrator.h"
#include <opencv2/opencv.hpp>
void testFunc() {
    ColorManagedCalibrator c;
    cv::Mat m1(3, 2, CV_32FC1);
    cv::Mat m2(2, 3, CV_32FC1);

    float num = 1;
    for (int row = 0; row < m1.rows; row++) {
        for (int col = 0; col < m1.cols; col++) {
            m1.at<float>(row, col) = num;
            num += 1;
        }
    }
    std::cout << "M1" << std::endl;
    c.display_matrix(&m1);
    num = 1;
    for (int row = 0; row < m2.rows; row++) {
        for (int col = 0; col < m2.cols; col++) {
            m2.at<float>(row, col) = num;
            num += 1;
        }
    }
    std::cout << "M2" << std::endl;
    c.display_matrix(&m2);

    cv::Mat m3 = m1 * m2;// *m2;
    std::cout << "M1" << std::endl;
    c.display_matrix(&m1);
    std::cout << "M2" << std::endl;
    c.display_matrix(&m2);
    std::cout << "M3" << std::endl;
    c.display_matrix(&m3);
}


int main() {
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
