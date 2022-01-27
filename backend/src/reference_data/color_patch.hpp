#ifndef COLOR_PATCH_H
#define COLOR_PATCH_H

#include <string>
#include <iostream>
#include "ref_data_defines.hpp"
#include "ref_data_array.hpp"
#include "illuminants.hpp"
#include "standard_observer.hpp"

class ColorPatch {
	enum ValueType {
		X,Y,Z,
		L,A,B
	};

public:
	ColorPatch(short row, short col, Illuminants* illum, StandardObserver* so);
	~ColorPatch();
	friend std::ostream& operator<<(std::ostream& os, const ColorPatch& cp) {
		os << cp.name_m << std::endl;
		return os;
	}
	void append(double value);
	std::string get_name()const;
	void set_name(std::string name);
	short get_row();
	short get_col();
	double get_ref_by_index(int index);
	double get_ref_by_wavelen(int wavelength);
	double get_x();
	double get_y();
	double get_z();
	double get_l();
	double get_a();
	double get_b();


private:
	std::string name_m;
	RefDataArray* reflectance = nullptr;
	Illuminants* illuminants = nullptr;
	StandardObserver* observer = nullptr;
	int index_m = 0;
	//Position of ColorPatch in ColorTarget
	short row;
	short col;
	// Tristimulus Values
	double *x = nullptr;
	double *y = nullptr;
	double *z = nullptr;
	//// CIELAB Value
	double *l = nullptr;
	double *a = nullptr;
	double *b = nullptr;

	double init_Tristimulus(ValueType type);
	StandardObserver::ValueType get_so_type(ValueType type);
	double calc_k_value();
	double sum_reflectance();
};

#endif // !COLOR_PATCH_H
