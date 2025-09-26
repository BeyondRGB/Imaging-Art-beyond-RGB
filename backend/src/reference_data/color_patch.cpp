#include <reference_data/color_patch.hpp>
#include <utils/color_conversions.hpp>

ColorPatch::ColorPatch(short row, short col, Illuminants* illum, StandardObserver* so, WhitePoints* white_pts) {
	this->row = row;
	this->col = col;
	this->reflectance = new RefDataArray(REFLECTANCE_SIZE);
	this->illuminants = illum;
	this->observer = so;
	this->white_pts = white_pts;
}

ColorPatch::~ColorPatch() {
	// Do not delete iluminants, observer, white_pts as they
	// are passed by reference into this class and are not owned by this class
	if(nullptr != this->reflectance)
		delete this->reflectance;
}

short ColorPatch::get_row() { return this->row; }
short ColorPatch::get_col() { return this->col; }

std::string ColorPatch::get_name() const { return this->name_m; }

void ColorPatch::set_name(std::string name) { this->name_m = name; }

void ColorPatch::append(double value) {
	try{
		this->reflectance->append(value);
	}
	catch (std::exception e) {
		throw e;
	}

}

void ColorPatch::init() {
	this->init_Tristumulus_values();
	this->init_CIELAB_values();
}

double ColorPatch::get_ref_by_index(int index) {
	if (index < REFLECTANCE_SIZE)
		return this->reflectance->get_by_index(index);
	else
		throw std::out_of_range("Index out of bounds");
}

double ColorPatch::get_ref_by_wavelen(int wavelength) {
	return this->get_ref_by_index(WAVELEN_TO_INDEX(wavelength));
}

double ColorPatch::get_x() {
	return this->x;
}

double ColorPatch::get_y() {
	return this->y;
}

double ColorPatch::get_z() {
	return this->z;
}

double ColorPatch::get_L() {
	return this->l;
}

double ColorPatch::get_a() {
	return this->a;
}

double ColorPatch::get_b() {
	return this->b;
}

double ColorPatch::calc_Tristimulus(ValueType type) {
	double k = this->calc_k_value();
	double sum = 0;
	double oberver_value;
	double illum_value;
	double reflectanc_value;
	StandardObserver::ValueType so_type = this->get_so_type(type);
	for (int i = 0; i < STANDARD_OBSERVER_SIZE; i++) {
		oberver_value = this->observer->value_by_index(so_type, i);
		illum_value = this->illuminants->value_by_index(i);
		reflectanc_value = this->reflectance->get_by_index(i);
		sum += oberver_value * illum_value * reflectanc_value;
	}
	return k * sum * SAMPLING_INCREMENT;
}

void ColorPatch::init_Tristumulus_values() {
	this->x = this->calc_Tristimulus(ValueType::X);
	this->y = this->calc_Tristimulus(ValueType::Y);
	this->z = this->calc_Tristimulus(ValueType::Z);
}

void ColorPatch::init_CIELAB_values() {
	double X = this->get_x();
	double Y = this->get_y();
	double Z = this->get_z();

	btrgb::XYZ_t xyz = {X, Y, Z};
	btrgb::Lab_t lab =  btrgb::xyz_2_Lab(xyz, this->white_pts);

	this->l = lab.L;
	this->a = lab.a;
	this->b = lab.b;
}

double ColorPatch::calc_k_value() {
	double so_x_ilum_sum = 0;
	StandardObserver::ValueType type = get_so_type(ValueType::Y);
	for (int i = 0; i < STANDARD_OBSERVER_SIZE; i++) {
		double oberver_value = this->observer->value_by_index(type, i);
		double illum_value = this->illuminants->value_by_index(i);
		so_x_ilum_sum += oberver_value * illum_value;
	}
	return 100 / (so_x_ilum_sum * SAMPLING_INCREMENT);
}

StandardObserver::ValueType ColorPatch::get_so_type(ValueType type) {
	switch (type) {
	case X:
		return StandardObserver::ValueType::X;
	case Y:
		return StandardObserver::ValueType::Y;
	case Z:
		return StandardObserver::ValueType::Z;
	default:
		return StandardObserver::ValueType::Y;

	}
}


