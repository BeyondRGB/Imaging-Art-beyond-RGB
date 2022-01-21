#include "ref_data.hpp"
#include <iostream>
#include <fstream>

RefData::RefData(std::string file_path) {
	this->read_in_data(file_path);
}

void RefData::read_in_data(std::string file_path) {
	std::cout << "Reading File: " << file_path << std::endl;
	std::ifstream ref_file(file_path);
	std::string line;
	while (std::getline(ref_file, line)) {
		std::cout << line << std::endl;
	}
	ref_file.close();
}