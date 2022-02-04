#ifndef COLOR_PATCH_H
#define COLOR_PATCH_H

#include <string>
#include <iostream>
#include "ref_data_defines.hpp"
#include "ref_data_array.hpp"
#include "illuminants.hpp"
#include "standard_observer.hpp"
#include "white_points.hpp"

/**
* Class that represents an individual collor patch in reference data
* Each color patch holds a RefDataArray that is red in as the RefData the patch 
* belongs to. Each color patch can compute a Tristimulus value(x,y,z)
* for itself. 
*
* ColorPatch will be able to compute CIELAB values (l,a,b), but
* this has not yet been implemented yet
*/
class ColorPatch {
	enum ValueType {
		X,Y,Z,
		L,A,B
	};

public:
	ColorPatch(short row, short col, Illuminants* illum, StandardObserver* so, WhitePoints* white_pts);
	~ColorPatch();
	/**
	* Overloaded  << operator. 
	*/
	friend std::ostream& operator<<(std::ostream& os, const ColorPatch& cp) {
		os << cp.name_m << std::endl;
		return os;
	}

	/**
	* Append value to RefDataArray that represents reflectance.
	* Throws an exception if attempting to add more values than the 
	* RefDataArray can hold.
	* This shoud only realy be called by RefData when reading in data
	* @param value: the numeric value to append
	*/
	void append(double value);

	/**
	* Gets the name of the CollorPatch
	* @return name of CollorPatch
	*/
	std::string get_name()const;

	/**
	* Sets the name of the collor patch
	* @param name: the string to set the name to
	*/
	void set_name(std::string name);

	/**
	* Gets the row the CollorPatch lives in
	* @return row
	*/
	short get_row();

	/**
	* Gets the colum the CollorPatch lives in
	* @return col
	*/
	short get_col();

	/**
	* Gets RefData from RefDataArray by index
	* @param index: the index to be used to retrive data from
	* @return reference data
	*/
	double get_ref_by_index(int index);

	/**
	* Gets RefData from RefDataArray by wavelength
	* @param wavelength: the wavelength to be used to retrive data for
	* @return reference data
	*/
	double get_ref_by_wavelen(int wavelength);

	/**
	* Get the x value for this CollorPatch
	* x is a calculated value that is only computed once.
	* After the first time it is computed x is stored as a member variable,
	* so that it wont need to be computed again latter
	* @return double x
	*/
	double get_x();

	/**
	* Get the y value for this CollorPatch
	* y is a calculated value that is only computed once.
	* After the first time it is computed t is stored as a member variable,
	* so that it wont need to be computed again latter
	* @return double y
	*/
	double get_y();

	/**
	* Get the z value for this CollorPatch
	* z is a calculated value that is only computed once.
	* After the first time it is computed z is stored as a member variable,
	* so that it wont need to be computed again latter
	* @return double z
	*/
	double get_z();

	/**
	* TODO these have not yet been implemneted yet 
	* as we dont know the math for them.
	* But when we do know this is where the l,a,b values shoule
	* come from
	*/
	double get_L();
	double get_a();
	double get_b();


private:
	// Name of the CollorPatch
	std::string name_m;
	RefDataArray* reflectance = nullptr;
	// Reference to the illuminants used for calucalted values
	Illuminants* illuminants = nullptr;
	// Reference to the StandardObserver used for calculated values
	StandardObserver* observer = nullptr;
	// Reference to WhitePoints used for calculated values
	WhitePoints* white_pts = nullptr;
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

	/**
	* Calculates the Tristimulus value of the given type
	* @param type: the ValueType::(x,y,z) to compute
	* @return: the Tristimulus value(x,y,z) computed
	*/
	double init_Tristimulus(ValueType type);

	/**
	* Calculates the f(x) value used for calculating
	* L*,a*,b* values
	* @param x: double X/Xn, Y/Yn, or Z/Zn 
	*	where X,Y,Z are the x,y,z values contained by this ColorPatch
	*	and Xn, Yn, Zn are the Xn, Yn, Zn from the WhitePoints
	* @return: computed double
	*/
	double lab_f(double x);
	
	/**
	* TODO these have not yet been implemneted yet
	* as we dont know the math for them.
	* But when we do know this is where the l,a,b values shoule
	* be computed
	*/
	//double init_CIELAB(ValueType type);

	/**
	* Convert Value type into a StandardObserver::ValueType
	*/
	StandardObserver::ValueType get_so_type(ValueType type);

	/**
	* Calculates the K value used in computing x,y,z values
	* This is called by init_tristimulus()
	* @return double k
	*/
	double calc_k_value();

};

#endif // !COLOR_PATCH_H
