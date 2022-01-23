#ifndef ILLUMINANTS_H
#define ILLUMINANTS_H

#include "csv_parser.hpp"
#include "ref_data_defines.hpp"
#include <string>

//#define ILLUMINANTS "../../res/ref_data/Illuminants.csv"

class Illuminants : public CSVParser {
	enum IlluminatType {
		A, D50, D65
	};

public:
	Illuminants();
	~Illuminants();
	double A_by_index(int index);
	double D50_by_index(int index);
	double D65_by_index(int index);
	double A_by_wavelen(int wavelen);
	double D50_by_wavelen(int wavelen);
	double D65_by_wavelen(int wavelen);

	double sum_a();
	double sum_d50();
	double sum_d65();
	double sum(double* array);

private:
	void init();
	double* illum_A = nullptr;
	double* illum_D50 = nullptr;
	double* illum_D65 = nullptr;
};
#endif // !ILLUMINANTS_H
