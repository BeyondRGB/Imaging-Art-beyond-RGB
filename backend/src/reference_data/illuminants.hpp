#ifndef ILLUMINANTS_H
#define ILLUMINANTS_H

#include "csv_parser.hpp"
#include "ref_data_defines.hpp"
#include "ref_data_array.hpp"
#include <string>

//#define ILLUMINANTS "../../res/ref_data/Illuminants.csv"

class Illuminants : public CSVParser {

public:
	enum IlluminantType {
		A, D50, D65
	};

	Illuminants(IlluminantType type);
	~Illuminants();
	double value_by_index(int index);
	double value_by_wavelen(int wavelen);
	//double A_by_index(int index);
	//double D50_by_index(int index);
	//double D65_by_index(int index);
	//double A_by_wavelen(int wavelen);
	//double D50_by_wavelen(int wavelen);
	//double D65_by_wavelen(int wavelen);

	//double sum_a();
	//double sum_d50();
	//double sum_d65();
	//double sum(double* array);

private:
	void init();
	RefDataArray* illum_A = nullptr;
	RefDataArray* illum_D50 = nullptr;
	RefDataArray* illum_D65 = nullptr;
	IlluminantType type;
};
#endif // !ILLUMINANTS_H
