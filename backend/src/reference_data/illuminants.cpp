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

