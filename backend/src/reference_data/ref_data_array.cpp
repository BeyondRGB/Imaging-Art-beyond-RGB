#include <reference_data/ref_data_array.hpp>

RefDataArray::RefDataArray(int size) {
	this->size = size;
	this->data = new double[size];
}

RefDataArray::~RefDataArray() {
	if (nullptr != this->data)
		delete[] data; /* array of double */
}

double RefDataArray::get_by_index(int index) {
	if (is_valid_index(index))
		return this->data[index];
	throw std::out_of_range("Index out of bounds");
}

double RefDataArray::get_by_wavelen(int wavelen) {
	return this->get_by_index(WAVELEN_TO_INDEX(wavelen));
}

void RefDataArray::append(double value) {
	if (this->curr_index < this->size) {
		this->data[this->curr_index++] = value;
	}
	else
		throw std::runtime_error("RefDataArray is Full");
}

bool RefDataArray::is_valid_index(int index) {
	return index < this->size;
}