#include "color_patch.hpp"

#include "data_manager.hpp"
#include "standard_observer.hpp"

ColorPatch::ColorPatch() {
	this->reflectance = new double[REFLECTANCE_SIZE];
}

ColorPatch::~ColorPatch() {
	std::cout << "ColorPatch Destructor" << std::endl;
	if(nullptr != this->reflectance)
		delete this->reflectance;
	//clean Tristimulus Values
	std::cout << "Tristimulus Values" << std::endl;
	if (nullptr != this->x)
		delete this->x;
	if (nullptr != this->y)
		delete this->y;
	if (nullptr != this->z)
		delete this->z;
	// Clean CIELAB Value
	std::cout << "CIELAB Values" << std::endl;
	if (nullptr != this->l)
		delete this->l;
	if (nullptr != this->a)
		delete this->a;
	if (nullptr != this->b)
		delete this->b;
	std::cout << "Done" << std::endl;

}

void ColorPatch::append(double value) {
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

//double ColorPatch::get_x() {
//	if (nullptr == this->x)
//		*this->x = this->init_Tristimulus(ValueType::X);
//	return *this->x;
//}
//
//double ColorPatch::get_y() {
//	if (nullptr == this->y)
//		*this->y = this->init_Tristimulus(ValueType::Y);
//	return *this->y;
//}
//
//double ColorPatch::get_z() {
//	if (nullptr == this->z)
//		*this->z = this->init_Tristimulus(ValueType::Z);
//	return *this->z;
//}
//
//double ColorPatch::init_Tristimulus(ValueType type) {
//	return 5;
//}
//
//double ColorPatch::get_so_value(ValueType type, int index) {
//	//TODO at this point its unclear which standard observer to use
//	StandardObserver* so = DataManager::get_instance()->get_observer_1931();
//	switch (type) {
//	case X:
//		return so->x_by_index(index);
//	case Y:
//		return so->y_by_index(index);
//	case Z:
//		return so->z_by_index(index);
//
//	}
//}


