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
	std::string header;
	std::getline(ref_file, header);
	std::cout << "ItemCount(" << header << "): " << this->count_line_items(header) << std::endl;
	//header = "Hello";
	std::cout << "Has_next(" << header << "):" << (this->has_next(header) ? "True" : "False") << std::endl;
	while (std::getline(ref_file, line)) {
		//std::cout << line << std::endl;
		this->pars_line(line, ",");
	}
	ref_file.close();
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