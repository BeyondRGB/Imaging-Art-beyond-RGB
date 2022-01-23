#ifndef STANDARD_OBSERVER_H
#define STANDARD_OBSERVER_H

#include <string>
#include "csv_parser.hpp"
#include "ref_data_defines.hpp"
#include "ref_data_array.hpp"

class StandardObserver : public CSVParser {


public:
	enum ObserverType {
		SO_1931,
		SO_1964
	};
	enum ValueType {
		X,Y,Z
	};
	StandardObserver(ObserverType type);
	~StandardObserver();
	void init(std::string file);
	double value_by_index(ValueType type, int index);
	double value_by_wavelen(ValueType type, int wavelen);
	//double x_by_index(int index);
	//double y_by_index(int index);
	//double z_by_index(int index);
	//double x_by_wavelen(int wavelen);
	//double y_by_wavelen(int wavelen);
	//double z_by_wavelen(int wavelen);
	//double sum_y();
	//double sum_x();
	//double sum_z();

private:
	RefDataArray* x = nullptr;
	RefDataArray* y = nullptr;
	RefDataArray* z = nullptr;
	//double sum(double* array);
};

#endif // !STANDARD_OBSERVER_H
