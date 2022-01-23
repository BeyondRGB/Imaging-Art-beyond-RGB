#ifndef STANDARD_OBSERVER_H
#define STANDARD_OBSERVER_H

#include <string>
#include "csv_parser.hpp"
#include "ref_data_defines.hpp"

class StandardObserver : public CSVParser {

	

	//TODO Im not currently sure how to do the relative pathing for this yet
	//TODO this will need to be fixed
	const std::string file_key[2] = {
		"C:\\Users\\ThinkPad41\\Documents\\GitHub\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\standard_Observer_1931.csv",
		"C:\\Users\\ThinkPad41\\Documents\\GitHub\\Imaging-Art-beyond-RGB\\backend\\res\\ref_data\\Standard_Observer_1964.csv"
	};

public:
	enum ObserverType {
		SO_1931,
		SO_1964
	};
	StandardObserver(ObserverType type);
	~StandardObserver();
	void init(std::string file);
	double x_by_index(int index);
	double y_by_index(int index);
	double z_by_index(int index);
	double x_by_wavelen(int wavelen);
	double y_by_wavelen(int wavelen);
	double z_by_wavelen(int wavelen);
	double sum_y();
	double sum_x();
	double sum_z();

private:
	double* x = nullptr;
	double* y = nullptr;
	double* z = nullptr;
	double sum(double* array);
};

#endif // !STANDARD_OBSERVER_H
