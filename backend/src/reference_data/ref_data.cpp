#include "ref_data.hpp"

RefData::RefData(std::string file_path) {
	this->read_in_data(file_path);
}

void RefData::read_in_data(std::string file_path) {
	std::cout << "Reading File: " << file_path << std::endl;
}