#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "utils/matrix_utils.hpp"
#include <opencv2/opencv.hpp>

using namespace btrgb::matrix_utils;
using Catch::Approx;

TEST_CASE("Matrix value insertion with doubles", "[matrix][utils]") {
    cv::Mat matrix = cv::Mat::zeros(3, 3, CV_64F);
    
    SECTION("Insert value into matrix") {
        insert_value<double>(matrix, 0, 0, 5.5);
        insert_value<double>(matrix, 1, 1, 10.5);
        insert_value<double>(matrix, 2, 2, 15.5);
        
        REQUIRE(matrix.at<double>(0, 0) == Approx(5.5));
        REQUIRE(matrix.at<double>(1, 1) == Approx(10.5));
        REQUIRE(matrix.at<double>(2, 2) == Approx(15.5));
    }
    
    SECTION("Overwrite existing value") {
        insert_value<double>(matrix, 1, 1, 100.0);
        REQUIRE(matrix.at<double>(1, 1) == Approx(100.0));
        
        insert_value<double>(matrix, 1, 1, 200.0);
        REQUIRE(matrix.at<double>(1, 1) == Approx(200.0));
    }
}

TEST_CASE("Matrix value insertion with integers", "[matrix][utils]") {
    cv::Mat matrix = cv::Mat::zeros(3, 3, CV_32S);
    
    SECTION("Insert integer values") {
        insert_value<int>(matrix, 0, 0, 5);
        insert_value<int>(matrix, 1, 1, 10);
        insert_value<int>(matrix, 2, 2, 15);
        
        REQUIRE(matrix.at<int>(0, 0) == 5);
        REQUIRE(matrix.at<int>(1, 1) == 10);
        REQUIRE(matrix.at<int>(2, 2) == 15);
    }
}

TEST_CASE("Matrix value insertion with floats", "[matrix][utils]") {
    cv::Mat matrix = cv::Mat::zeros(2, 2, CV_32F);
    
    SECTION("Insert float values") {
        insert_value<float>(matrix, 0, 0, 3.14f);
        insert_value<float>(matrix, 1, 1, 2.71f);
        
        REQUIRE(matrix.at<float>(0, 0) == Approx(3.14f));
        REQUIRE(matrix.at<float>(1, 1) == Approx(2.71f));
    }
}

