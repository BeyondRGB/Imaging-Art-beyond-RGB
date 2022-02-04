#include "white_points.hpp"

WhitePoints::WhitePoints(ObserverType observer, IlluminantType illuminant) {
	this->observer = observer;
	this->illuminant = illuminant;
	this->init_data_storage();
	//Xn
	values[ValueType::Xn][IlluminantType::A][ObserverType::SO_1931] = 109.846606945637;
	values[ValueType::Xn][IlluminantType::D50][ObserverType::SO_1931] = 96.4211994421199;
	values[ValueType::Xn][IlluminantType::D65][ObserverType::SO_1931] = 95.0428545377181;

	values[ValueType::Xn][IlluminantType::A][ObserverType::SO_1964] = 111.142040695669;
	values[ValueType::Xn][IlluminantType::D50][ObserverType::SO_1964] = 96.7206275033378;
	values[ValueType::Xn][IlluminantType::D65][ObserverType::SO_1964] = 94.809667673716;

	//Yn
	values[ValueType::Yn][IlluminantType::A][ObserverType::SO_1931] = 100;
	values[ValueType::Yn][IlluminantType::D50][ObserverType::SO_1931] = 100;
	values[ValueType::Yn][IlluminantType::D65][ObserverType::SO_1931] = 100;

	values[ValueType::Yn][IlluminantType::A][ObserverType::SO_1964] = 100;
	values[ValueType::Yn][IlluminantType::D50][ObserverType::SO_1964] = 100;
	values[ValueType::Yn][IlluminantType::D65][ObserverType::SO_1964] = 100;

	//Zn
	values[ValueType::Zn][IlluminantType::A][ObserverType::SO_1931] = 35.5822800343601;
	values[ValueType::Zn][IlluminantType::D50][ObserverType::SO_1931] = 82.5188284518829;
	values[ValueType::Zn][IlluminantType::D65][ObserverType::SO_1931] = 108.890037079812;

	values[ValueType::Zn][IlluminantType::A][ObserverType::SO_1964] = 35.1997832191949;
	values[ValueType::Zn][IlluminantType::D50][ObserverType::SO_1964] = 81.4280151312862;
	values[ValueType::Zn][IlluminantType::D65][ObserverType::SO_1964] = 107.305135951661;
}

WhitePoints::~WhitePoints() {
	for (int row = 0; row < VALUE_COUNT; row++) {
		for (int col = 0; col < ILLUMINANT_COUNT; col++) {
			delete[] values[row][col];
		}
		delete[] values[row];
	}
	delete[] values;
}

void WhitePoints::init_data_storage() {
	values = new double**[VALUE_COUNT];
	for (int row = 0; row < VALUE_COUNT; row++) {
		values[row] = new double*[ILLUMINANT_COUNT];
		for (int col = 0; col < ILLUMINANT_COUNT; col++) {
			values[row][col] = new double[OBSERVER_COUNT];
		}
	}

}

double WhitePoints::get_white_point(WhitePoints::ValueType type) {
	return values[type][this->illuminant][this->observer];
}