#include "illuminants.hpp"

Illuminants::Illuminants() {
	this->init();
}

Illuminants::~Illuminants() {
	if (nullptr != this->illuminant_A)
		delete this->illuminant_A;
	if (nullptr != this->D50)
		delete this->D50;
	if (nullptr != this->D65)
		delete this->D65;
}

void Illuminants::init() {
	std::cout << "Init Illuminants" << std::endl;

	try {
		bool is_open = this->open_file(ILLUMINANTS_FILE_PATH);
		std::cout << "Did I get here" << std::endl;

		std::cout << this->get_next_line() << std::endl;

		this->close_file();
	}
	catch (std::exception e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}