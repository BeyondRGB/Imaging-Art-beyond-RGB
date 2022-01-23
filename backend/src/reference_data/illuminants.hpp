#ifndef ILLUMINANTS_H
#define ILLUMINANTS_H

#include "csv_parser.hpp"
#include "ref_data_defines.hpp"

//#define ILLUMINANTS "../../res/ref_data/Illuminants.csv"

class Illuminants : public CSVParser {

public:
	Illuminants();
	~Illuminants();
	//double sum_a();
	//double sum_d50();
	//double sum_d65();

private:
	void init();
	double* illuminant_A = nullptr;
	double* D50 = nullptr;
	double* D65 = nullptr;
};
#endif // !ILLUMINANTS_H
