#include "illuminants.hpp"

Illuminants::Illuminants(IlluminantType type) {
	this->init();
	this->type = type;
}

Illuminants::~Illuminants() {
	if (nullptr != this->illum_A)
		delete this->illum_A;
	if (nullptr != this->illum_D50)
		delete this->illum_D50;
	if (nullptr != this->illum_D65)
		delete this->illum_D65;
}

void Illuminants::init() {
	bool is_open = this->open_file(ILLUMINANTS_FILE_PATH);
	this->illum_A = new RefDataArray(ILLUMINATNTS_SIZE);
	this->illum_D50 = new RefDataArray(ILLUMINATNTS_SIZE);
	this->illum_D65 = new RefDataArray(ILLUMINATNTS_SIZE);
	this->get_next_line();//ignore header
	for (int i = 0; i < ILLUMINATNTS_SIZE; i++) {
		std::string line = this->get_next_line();
		// Ignor wavelength
		this->get_next<double>(line);
		this->illum_A->append(this->get_next<double>(line));
		this->illum_D50->append(this->get_next<double>(line));
		this->illum_D65->append(this->get_next<double>(line));

	}
	this->close_file();
}

double Illuminants::value_by_index(int index) {
	switch (this->type) {
	case A:
		return this->illum_A->get_by_index(index);
	case D50:
		return this->illum_D50->get_by_index(index);
	case D65:
		return this->illum_D65->get_by_index(index);
	default:
		return this->illum_D50->get_by_index(index);
	}
}

double Illuminants::value_by_wavelen(int wavelen) {
	return this->value_by_index(WAVELEN_TO_INDEX(wavelen));
}

//double Illuminants::A_by_index(int index) {
//	if (index < ILLUMINATNTS_SIZE) {
//		return this->illum_A[index];
//	}
//	throw std::out_of_range("Index out of bounds");
//}
//
//double Illuminants::D50_by_index(int index) {
//	if (index < ILLUMINATNTS_SIZE) {
//		return this->illum_D50[index];
//	}
//	throw std::out_of_range("Index out of bounds");
//}
//
//double Illuminants::D65_by_index(int index) {
//	if (index < ILLUMINATNTS_SIZE) {
//		return this->illum_D65[index];
//	}
//	throw std::out_of_range("Index out of bounds");
//}
//
//double Illuminants::A_by_wavelen(int wavelen) {
//	return this->A_by_index(WAVELEN_TO_INDEX(wavelen));
//}
//
//double Illuminants::D50_by_wavelen(int wavelen) {
//	return this->D50_by_wavelen(WAVELEN_TO_INDEX(wavelen));
//}
//
//double Illuminants::D65_by_wavelen(int wavelen) {
//	return this->D50_by_wavelen(WAVELEN_TO_INDEX(wavelen));
//}
//
//double Illuminants::sum_a() {
//	return this->sum(this->illum_A);
//}
//
//double Illuminants::sum_d50() {
//	return this->sum(this->illum_D50);
//}
//
//double Illuminants::sum_d65() {
//	return this->sum(this->illum_D65);
//}
//
//double Illuminants::sum(double* array) {
//	double sum = 0;
//	for (int i = 0; i < ILLUMINATNTS_SIZE; i++) {
//		sum += array[i];
//	}
//	return sum;
//}
