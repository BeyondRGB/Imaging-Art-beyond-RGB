#ifndef ILLUMINANTS_H
#define ILLUMINANTS_H

#include "csv_parser.hpp"
#include "ref_data_defines.hpp"
#include "ref_data_array.hpp"
#include <string>

/**
* Class that holds and provides Illuminant values 
* used for calculating Reference Data
*/
class Illuminants : public CSVParser {

public:
	enum IlluminantType {
		A, D50, D65
	};

	Illuminants(IlluminantType type);
	~Illuminants();

	/**
	* Get the illuminant value by index
	* @param index: the index of the value beeing retrived
	* @return: illuminant value
	*/
	double value_by_index(int index);

	/**
	* Get the illuminant value by wavelength
	* @param index: the wavelength of the value beeing retrived
	* @return: illuminant value
	*/
	double value_by_wavelen(int wavelen);

private:

	/**
	* Initialize Illuminant data. This will initialize a RefDataArray
	* for each IlluminantType due to all data existing in one file
	*/
	void init();
	RefDataArray* illum_A = nullptr;
	RefDataArray* illum_D50 = nullptr;
	RefDataArray* illum_D65 = nullptr;
	IlluminantType type;
};
#endif // !ILLUMINANTS_H
