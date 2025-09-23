#ifndef STANDARD_OBSERVER_H
#define STANDARD_OBSERVER_H

#include <string>
#include <utils/csv_parser.hpp>
#include <reference_data/ref_data_array.hpp>

/**
* Class to hold and provide StandardObserver values
* This is used to compute reference data values
*/
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

	/**
	* Get StandardObserver value by index
	* @param type: the ValueType(x,y,z) to get value for
	* @param index: the index to get value from
	* @return: the observer value
	*/
	double value_by_index(ValueType type, int index);

	/**
	* Get StandardObserver value by wavelength
	* @param type: the ValueType(x,y,z) to get value for
	* @param index: the wavelength to get value from
	* @return: the observer value
	*/
	double value_by_wavelen(ValueType type, int wavelen);

private:
	/**
	* Initialize the StandardObserver data
	* This will read data in to RefDataArray*s x,y,z
	*/
	void init(std::string file);
	RefDataArray* x = nullptr;
	RefDataArray* y = nullptr;
	RefDataArray* z = nullptr;
	//double sum(double* array);
};

#endif // !STANDARD_OBSERVER_H
