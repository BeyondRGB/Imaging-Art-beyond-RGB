#include "ref_data.hpp"
#include <iostream>
#include <fstream>

RefData::RefData(std::string file) {
	std::string path = REF_DATA_PATH;
	this->f_name = file;
	this->read_in_data(path + file);
}

RefData::~RefData() {
	for (int row = 0; row < this->row_count; row++) {
		for (int col = 0; col < this->col_count; col++) {
			delete this->color_patches[row][col];
		}
		delete this->color_patches[row];
	}
	delete this->color_patches;
}

int RefData::get_row_size() {
	return this->row_count;
}

int RefData::get_col_size() {
	return this->col_count;
}

ColorPatch* RefData::get_color_patch(int row, int col) {
	if(row < this->row_count && col < this->col_count)
		return this->color_patches[row][col];
	throw std::out_of_range("Index out of bounds");
}

ColorPatch* RefData::get_white_patch() {
	ColorPatch* current_best = this->get_color_patch(0, 0);
	for (int row = 0; row < this->row_count; row++) {
		for (int col = 0; col < this->col_count; col++) {
			ColorPatch* cp = this->get_color_patch(row, col);
			if (cp->get_y() > current_best->get_y()) {
				current_best = cp;
			}
		}
	}
	return current_best;
}

void RefData::read_in_data(std::string file_path) {
	this->open_file(file_path);
	std::string header = this->get_next_line();
	this->identify_data_size(header);
	this->init_data_storage();
	this->pars_header(header);
	
	while (this->has_next_line()) {
		std::string line = this->get_next_line();
		this->pars_line(line);
	}

	//for (int row = 0; row < row_count; row++) {
	//	for (int col = 0; col < col_count; col++) {
	//		std::cout << *this->color_patches[row][col]  << std::endl;
	//	}
	//	std::cout << std::endl;
	//}
	//int wave = 730;
	//int i = 35;
	//std::cout << "Wavelen: " << wave << " index: " << WAVELEN_TO_INDEX(wave) << std::endl;
	//std::cout << "index: " << i << " wavelen: " << INDEX_TO_WAVELEN(i) << std::endl;

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
	this->row_count = row_count; 
	this->col_count = col_count;

}

void RefData::init_data_storage() {
	this->color_patches = new ColorPatch ** [this->row_count];
	for (int row = 0; row < this->row_count; row++) {
		this->color_patches[row] = new ColorPatch*[this->col_count];
		for (int col = 0; col < this->col_count; col++) {
			this->color_patches[row][col] = new ColorPatch(row, col);
		}
	}
}

std::string RefData::get_col_id(std::string header_item) {
	size_t pos = header_item.find(":");
	return header_item.substr(0, pos);
}

void RefData::output_xyz() {
	std::string header = "ValueType";
	std::string y_values = "Y";
	std::string x_values = "X";
	std::string z_values = "Z";
	for (int col = 0; col < this->get_col_size(); col++) {
		for (int row = 0; row < this->get_row_size(); row++) {
			std::string comma = ",";
			ColorPatch* cp = this->get_color_patch(row, col);
			header += comma + cp->get_name();
			y_values += comma + std::to_string(cp->get_y());
			x_values += comma + std::to_string(cp->get_x());
			z_values += comma + std::to_string(cp->get_z());
		}
	}
	std::cout << this->f_name << std::endl;
	std::cout << header << std::endl;
	std::cout << x_values << std::endl;
	std::cout << y_values << std::endl;
	std::cout << z_values << std::endl;
	ColorPatch* cp = this->get_white_patch();
	std::cout << "White Patch," << cp->get_name() << std::endl;
	std::cout << "Y Value, " << cp->get_y() << ",Row," << cp->get_row() << ",col," << cp->get_col() << std::endl << std::endl;
}