#include "ref_data.hpp"
#include <iostream>
#include <fstream>

RefData::RefData(std::string file_path) {
	this->read_in_data(file_path);
}

RefData::~RefData() {
	for (int i = 0; i < this->row_count; i++) {
		delete this->color_patches[i];
	}
	delete this->color_patches;
}

void RefData::read_in_data(std::string file_path) {
	std::cout << "Reading File: " << file_path << std::endl;
	this->open_file(file_path);
	std::string header = this->get_next_line();
	this->identify_data_size(header);
	this->init_data_storage();
	this->pars_header(header);
	std::cout << "ColorPatches" << std::endl;
	for (int row = 0; row < row_count; row++) {
		for (int col = 0; col < col_count; col++) {
			std::cout << this->color_patches[row][col] << " ";
		}
		std::cout << std::endl;
	}
	//while (this->has_next_line()) {
		//std::string line = this->get_next_line();
		//this->pars_line(line);
	//}
	this->close_file();
}

void RefData::pars_line(std::string line) {
	while (this->has_next(line)) {
		int item = this->get_next<int>(line);
		std::cout << "Item: " << item << std::endl;
	}
}

void RefData::pars_header(std::string header) {
	for (int row = 0; row < row_count; row++) {
		for (int col = 0; col < col_count; col++) {
			std::string item = this->get_next<std::string>(header);
			this->color_patches[row][col] = item;
		}
	}
}

void RefData::identify_data_size(std::string header) {
	std::string h_coppy = header;
	// Subtract 1 to ignore wavelength
	int item_count = this->count_line_items(h_coppy) - 1;
	std::string current_col_id = "";
	// Start at -1 to ignor wavelength col
	int col_count = -1;
	int i = 0;
	while (this->has_next(h_coppy)) {
		std::string item = this->get_next<std::string>(h_coppy);
		std::string col_id = this->get_col_id(item);
		if (current_col_id != col_id || current_col_id.length() == 0) {
			col_count++;
			current_col_id = col_id;
		}
	}
	int row_count = item_count / col_count;
	std::cout << "RowCount: " << row_count << " ColCount: " << col_count << std::endl;
	this->row_count = row_count; 
	this->col_count = col_count;
}

void RefData::init_data_storage() {
	this->color_patches = new std::string * [this->row_count];
	for (int i = 0; i < this->row_count; i++) {
		this->color_patches[i] = new std::string[this->col_count];
	}
}

std::string RefData::get_col_id(std::string header_item) {
	size_t pos = header_item.find(":");
	return header_item.substr(0, pos);
}