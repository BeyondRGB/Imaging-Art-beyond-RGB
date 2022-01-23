#include "ref_data.hpp"
#include <iostream>
#include <fstream>

RefData::RefData(std::string file_path) {
	this->read_in_data(file_path);
}

RefData::~RefData() {
	std::cout << "RefData Destructor" << std::endl;
	for (int row = 0; row < this->row_count; row++) {
		for (int col = 0; col < this->col_count; col++) {
			std::cout << "RefData Delete row: " << row << " col: " << col << std::endl;
			delete this->color_patches[row][col];
		}
		std::cout << "RefData Delete Row: " << row << std::endl;
		delete this->color_patches[row];
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
	
	while (this->has_next_line()) {
		std::string line = this->get_next_line();
		this->pars_line(line);
	}

	for (int row = 0; row < row_count; row++) {
		for (int col = 0; col < col_count; col++) {
			std::cout << *this->color_patches[row][col]  << std::endl;
		}
		std::cout << std::endl;
	}
	int wave = 730;
	int i = 35;
	std::cout << "Wavelen: " << wave << " index: " << WAVELEN_TO_INDEX(wave) << std::endl;
	std::cout << "index: " << i << " wavelen: " << INDEX_TO_WAVELEN(i) << std::endl;

	this->close_file();
}

void RefData::pars_line(std::string line) {
	// Ignor Wavlen col
	this->get_next<int>(line);
	for (int col = 0; col < col_count; col++) {
		for (int row = 0; row < row_count; row++) {
			double item = this->get_next<double>(line);
			this->color_patches[row][col]->append(item);
		}
	}
}

void RefData::pars_header(std::string header) {
	// Ignor wavelength col
	this->get_next<std::string>(header);
	for (int col = 0; col < col_count; col++) {
		for (int row = 0; row < row_count; row++) {
			std::string token = this->get_next<std::string>(header);
			this->color_patches[row][col]->set_name(token);
		}
		std::cout << std::endl;
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
	this->color_patches = new ColorPatch ** [this->row_count];
	for (int row = 0; row < this->row_count; row++) {
		this->color_patches[row] = new ColorPatch*[this->col_count];
		for (int col = 0; col < this->col_count; col++) {
			this->color_patches[row][col] = new ColorPatch();
		}
	}
}

std::string RefData::get_col_id(std::string header_item) {
	size_t pos = header_item.find(":");
	return header_item.substr(0, pos);
}