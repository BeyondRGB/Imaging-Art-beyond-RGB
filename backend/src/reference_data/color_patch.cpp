#include "color_patch.hpp"

ColorPatch::ColorPatch() {
	this->reflectance = new double[REFLECTANCE_SIZE];
}
//ColorPatch::ColorPatch(std::string name, int size) {
//	std::cout << "Name: " << name << " Size: " << size << std::endl;
//	this->reflectance = new double[size];
//	this->size_m = size;
//}

ColorPatch::~ColorPatch() {
	delete this->reflectance;
}

//std::ostream& ColorPatch::operator<<(std::ostream& os, const ColorPatch& cp) {
//	return os << cp.name_m;
//}

//std::ostream& ColorPatch::operator<<(std::ostream& os, const ColorPatch& cp) {
//	os << cp.name_m;
//	return os;
//}

void ColorPatch::append(double value) {
	//std::cout << "Appending: " << value << std::endl;
	//std::cout << "Size: " << this->size_m << " index: " << this->index_m << std::endl;
	if (this->index_m < REFLECTANCE_SIZE) {
		this->reflectance[this->index_m++] = value;
	}
	else
		throw std::exception("ColorPatch is Full");

}

double ColorPatch::get_ref_by_index(int index) {
	if (index < REFLECTANCE_SIZE)
		return this->reflectance[index];
	else
		throw std::out_of_range("Index out of bounds");
}

double ColorPatch::get_ref_by_wavelen(int wavelength) {
	std::cout << this->reflectance;
	return this->get_ref_by_index(WAVELEN_TO_INDEX(wavelength));
}

//std::string ColorPatch::get_name() {
//	return name_m;
//}