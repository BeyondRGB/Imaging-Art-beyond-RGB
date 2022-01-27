#include "color_patch.hpp"

//#include "data_manager.hpp"
//#include "standard_observer.hpp"
//#include "illuminants.hpp"

ColorPatch::ColorPatch(short row, short col, Illuminants* illum, StandardObserver* so) {
	this->row = row;
	this->col = col;
	this->reflectance = new RefDataArray(REFLECTANCE_SIZE);
	this->illuminants = illum;
	this->observer = so;
}

ColorPatch::~ColorPatch() {
	if(nullptr != this->reflectance)
		delete this->reflectance;
	//clean Tristimulus Values
	if (nullptr != this->x)
		delete this->x;
	if (nullptr != this->y)
		delete this->y;
	if (nullptr != this->z)
		delete this->z;
	// Clean CIELAB Value
	if (nullptr != this->l)
		delete this->l;
	if (nullptr != this->a)
		delete this->a;
	if (nullptr != this->b)
		delete this->b;

}

short ColorPatch::get_row() { return this->row; }
short ColorPatch::get_col() { return this->col; }

std::string ColorPatch::get_name() const { return this->name_m; }

void ColorPatch::set_name(std::string name) { this->name_m = name; }

void ColorPatch::append(double value) {
	if (this->index_m < REFLECTANCE_SIZE) {
		this->reflectance->append(value);
	}
	else
		throw std::exception("ColorPatch is Full");

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
	if (nullptr == this->x) {
		this->x = new double;
		*this->x = this->init_Tristimulus(ValueType::X);
	}
	return *this->x;
}

double ColorPatch::get_y() {
	if (nullptr == this->y) {
		this->y = new double;
		*this->y = this->init_Tristimulus(ValueType::Y);
	}
	return *this->y;
}

double ColorPatch::get_z() {
	if (nullptr == this->z) {
		this->z = new double;
		*this->z = this->init_Tristimulus(ValueType::Z);
	}
	return *this->z;
}

double ColorPatch::init_Tristimulus(ValueType type) {
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

double ColorPatch::sum_reflectance() {
	double sum = 0;
	for (int i = 0; i < REFLECTANCE_SIZE; i++) {
		sum += this->reflectance->get_by_index(i);
	}
	return sum;
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


