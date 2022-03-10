#include "ref_data.hpp"
#include <iostream>
#include <fstream>
#include "server/globals_siglton.hpp"



RefData::RefData(std::string file, IlluminantType illum_type, ObserverType so_type) {
	this->observer = new StandardObserver(so_type);
	this->illuminants = new Illuminants(illum_type);
	this->white_pts = new WhitePoints(so_type, illum_type);
	std::string path = REF_DATA_PATH;
	this->f_name = file;
	this->read_in_data(path + file);
	this->init_color_patches();
}

RefData::~RefData() {
	if (nullptr != this->observer) {
		delete this->observer;
	}
	if (nullptr != this->illuminants) {
		delete this->illuminants;
	}
	if (nullptr != this->white_pts) {
		delete this->white_pts;
	}
	for (int row = 0; row < this->row_count; row++) {
		for (int col = 0; col < this->col_count; col++) {
			delete this->color_patches[row][col];
		}
		delete this->color_patches[row];
	}
	delete this->color_patches;
}

int RefData::get_row_count() {
	return this->row_count;
}

int RefData::get_col_count() {
	return this->col_count;
}

double RefData::get_x(int row, int col) {
	ColorPatch* cp = this->get_color_patch(row, col);
	return cp->get_x();
}
double RefData::get_y(int row, int col) {
	ColorPatch* cp = this->get_color_patch(row, col);
	return cp->get_y();
}
double RefData::get_z(int row, int col) {
	ColorPatch* cp = this->get_color_patch(row, col);
	return cp->get_z();
}

double RefData::get_L(int row, int col) {
	ColorPatch* cp = this->get_color_patch(row, col);
	return cp->get_L();
}

double RefData::get_a(int row, int col) {
	ColorPatch* cp = this->get_color_patch(row, col);
	return cp->get_a();
}

double RefData::get_b(int row, int col) {
	ColorPatch* cp = this->get_color_patch(row, col);
	return cp->get_b();
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

int RefData::get_white_patch_row() {
	return this->get_white_patch()->get_row();
}

int RefData::get_white_patch_col() {
	return this->get_white_patch()->get_col();
}

WhitePoints* RefData::get_white_pts(){
	return this->white_pts;
}

void RefData::read_in_data(std::string file_path) {
	if( !this->open_file(file_path) ) 
		throw std::runtime_error("[ref_data.cpp] Failed to open file: " + file_path);
	std::string header = this->get_next_line();
	this->identify_data_size(header);
	this->init_data_storage();
	this->pars_header(header);
	
	while (this->has_next_line()) {
		std::string line = this->get_next_line();
		this->pars_line(line);
	}
	this->close_file();
}

void RefData::pars_line(std::string line) {
	// Ignore Wavlen col
	this->get_next<int>(line);
	for (int col = 0; col < col_count; col++) {
		for (int row = 0; row < row_count; row++) {
			double item = this->get_next<double>(line);
			this->color_patches[row][col]->append(item);
		}
	}
}

void RefData::pars_header(std::string header) {
	// Ignore wavelength col
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
			this->color_patches[row][col] = new ColorPatch(row, col, this->illuminants, this->observer, this->white_pts);
		}
	}
}

std::string RefData::get_col_id(std::string header_item) {
	size_t pos = header_item.find(":");
	return header_item.substr(0, pos);
}

void RefData::init_color_patches() {
	for (int row = 0; row < row_count; row++) {
		for (int col = 0; col < col_count; col++) {
			this->color_patches[row][col]->init();
		}
	}
}

void RefData::output_xyz() {
	std::string header = "ValueType";
	std::string y_values = "Y";
	std::string x_values = "X";
	std::string z_values = "Z";
	std::string L_values = "L*";
	std::string a_values = "a*";
	std::string b_values = "b*";
	std::string comma = ",";
	for (int col = 0; col < this->get_col_count(); col++) {
		for (int row = 0; row < this->get_row_count(); row++) {
			ColorPatch* cp = this->get_color_patch(row, col);
			header += comma + cp->get_name();
			y_values += comma + std::to_string(cp->get_y());
			x_values += comma + std::to_string(cp->get_x());
			z_values += comma + std::to_string(cp->get_z());
			L_values += comma + std::to_string(cp->get_L());
			a_values += comma + std::to_string(cp->get_a());
			b_values += comma + std::to_string(cp->get_b());
		}
	}
	std::cout << this->f_name << std::endl;
	std::cout << "Xn: " << this->white_pts->get_white_point(WhitePoints::ValueType::Xn);
	std::cout << comma << " Yn: " << this->white_pts->get_white_point(WhitePoints::ValueType::Yn);
	std::cout << comma << " Zn: " << this->white_pts->get_white_point(WhitePoints::ValueType::Zn) << std::endl;

	std::cout << header << std::endl;
	std::cout << x_values << std::endl;
	std::cout << y_values << std::endl;
	std::cout << z_values << std::endl << std::endl;
	std::cout << L_values << std::endl;
	std::cout << a_values << std::endl;
	std::cout << b_values << std::endl << std::endl;
	ColorPatch* cp = this->get_white_patch();
	std::cout << "White Patch," << cp->get_name() << std::endl;
	std::cout << "Y Value, " << cp->get_y() << ",Row," << cp->get_row() << ",Col," << cp->get_col() << std::endl << std::endl << std::endl;
}

#include "utils/calibration_util.hpp"
cv::Mat RefData::as_matrix(){
	int col_count = this->row_count * this->col_count;
	cv::Mat ref_data = cv::Mat_<double>(REFLECTANCE_SIZE, col_count, CV_32FC1);
	for(int mat_row = 0; mat_row < ref_data.rows; mat_row++){
		int wave_len = INDEX_TO_WAVELEN(mat_row);
		// The Color patches are stored in 
			// A1, B1, C1, ..., K1
			// A2, B2, C2, ..., K2
			// ..., ..., ..., ..., ...
			// Ak, Bk, Ck, ..., Kk
		for(int row = 0; row < this->row_count; row++){
			for(int col = 0; col < this->col_count; col++){
				int mat_col = col + row * this->col_count;
				ColorPatch *cp = this->get_color_patch(row, col);
				double reflectance_value = cp->get_ref_by_wavelen(wave_len);
				// std::cout << cp->get_name() << ": " << cp->get_ref_by_wavelen(wave_len) << " mat_col:" << mat_col << " wavelen: " << wave_len  << std::endl;
				ref_data.at<double>(mat_row, mat_col) = reflectance_value;
				// btrgb::calibration::enter_to_continue();
			}
			// std::cout << std::endl << std::endl;
		}
	}

	return ref_data;
}
