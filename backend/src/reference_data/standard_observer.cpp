#include "standard_observer.hpp"

StandardObserver::StandardObserver(ObserverType type) {
	if (type == ObserverType::SO_1931)
		this->init(STANDARD_OBSERVER_1931_PATH);
	if (type == ObserverType::SO_1964)
		this->init(STANDARD_OBSERVER_1964_PATH);
}

StandardObserver::~StandardObserver() {
	if (nullptr != this->x) {
		delete this->x;
	}
	if (nullptr != this->y)
		delete this->y;
	if (nullptr != this->z)
		delete this->z;
}

void StandardObserver::init(std::string file) {
	this->x = new double[STANDARD_OBSERVER_SIZE];
	this->y = new double[STANDARD_OBSERVER_SIZE];
	this->z = new double[STANDARD_OBSERVER_SIZE];
	std::cout << "Generateing StandardObserver" << std::endl;
	this->open_file(file);
	// Ignore header
	this->get_next_line();
	for(int i = 0; i < STANDARD_OBSERVER_SIZE; i++) {
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

double StandardObserver::sum_y() {
	return this->sum(this->y);
}

double StandardObserver::sum_x() {
	return this->sum(this->x);
}

double StandardObserver::sum_z() {
	return this->sum(this->z);
}

double StandardObserver::sum(double* array) {
	double sum = 0;
	for (int i = 0; i < STANDARD_OBSERVER_SIZE; i++) {
		sum += array[i];
	}
	return sum;
}