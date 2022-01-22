#include "ref_data.hpp"
#include <iostream>
#include <fstream>

RefData::RefData(std::string file_path) {
	this->read_in_data(file_path);
}

void RefData::read_in_data(std::string file_path) {
	std::cout << "Reading File: " << file_path << std::endl;
	this->open_file(file_path);
	std::string header = this->get_next_line();
	std::cout << "Header: " << header << std::endl;
	while (this->has_next_line()) {
		std::string line = this->get_next_line();
		std::cout << "Line: " << line << std::endl;
	}
	this->close_file();
}

void RefData::pars_line(std::string line, std::string delimiter) {

	//std::string token;
	//size_t pos = line.find(delimiter);
	//int i = 0;
	//while (pos != std::string::npos) {
	//	token = line.substr(0, pos);
	//	std::cout << i << ": --" << token << " -- " << std::endl;
	//	// Remove current token from line
	//	line.erase(0, pos + delimiter.length());
	//	pos = line.find(delimiter);
	//	i++;
	//}
	//token = line.substr(0);
	//std::cout << i << ": --" << token << " -- " << std::endl;
}