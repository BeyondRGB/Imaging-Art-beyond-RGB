#include "standard_observer.hpp"

StandardObserver::StandardObserver(ObserverType type) {
	this->init(file_key[type]);
}

StandardObserver::~StandardObserver() {
	std::cout << "Destructor StandardObserver x" << std::endl;
	if (nullptr != this->x) {
		std::cout << "Deleting x" << std::endl;
		delete this->x;
	}

	std::cout << "Destructor StandardObserver y" << std::endl;
	if (nullptr != this->y)
		delete this->y;

	std::cout << "Destructor StandardObserver z" << std::endl;
	if (nullptr != this->z)
		delete this->z;
}

void StandardObserver::init(std::string file) {
	this->x = new double[REFLECTANCE_SIZE];
	this->y = new double[REFLECTANCE_SIZE];
	this->z = new double[REFLECTANCE_SIZE];
	std::cout << "Generateing StandardObserver" << std::endl;
	this->open_file(file);
	// Ignore header
	this->get_next_line();
	for(int i = 0; i < REFLECTANCE_SIZE; i++) {
		std::string line = this->get_next_line();
		// Ignor wavelength
		this->get_next<double>(line);
		this->x[i] = this->get_next<double>(line);
		this->y[i] = this->get_next<double>(line);
		this->z[i] = this->get_next<double>(line);
	}
	this->close_file();
}

double StandardObserver::x_by_index(int index) {
	if (index < REFLECTANCE_SIZE)
		return this->x[index];
	else
		throw std::out_of_range("Index out of bounds");
}

double StandardObserver::y_by_index(int index) {
	if (index < REFLECTANCE_SIZE)
		return this->y[index];
	else
		throw std::out_of_range("Index out of bounds");
}

double StandardObserver::z_by_index(int index) {
	if (index < REFLECTANCE_SIZE)
		return this->z[index];
	else
		throw std::out_of_range("Index out of bounds");
}

double StandardObserver::x_by_wavelen(int wavelen) {
	return this->x_by_index(WAVELEN_TO_INDEX(wavelen));
}

double StandardObserver::y_by_wavelen(int wavelen) {
	return this->y_by_index(WAVELEN_TO_INDEX(wavelen));
}

double StandardObserver::z_by_wavelen(int wavelen) {
	return this->z_by_index(WAVELEN_TO_INDEX(wavelen));
}
