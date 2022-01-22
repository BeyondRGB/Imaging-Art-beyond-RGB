#ifndef COLOR_PATCH_H
#define COLOR_PATCH_H

#include <string>
#include <iostream>

#define MAX_WAVE_LEN 730 // max wave length value
#define MIN_WAVE_LEN 380 // min wave length value
#define WAVE_LEN_INC 10 // wave length increments

#define WAVELEN_TO_INDEX(wavlen) (wavlen - MIN_WAVE_LEN) / WAVE_LEN_INC
#define INDEX_TO_WAVELEN(index) (index * WAVE_LEN_INC) + MIN_WAVE_LEN 

#define REFLECTANCE_SIZE 1 + (MAX_WAVE_LEN - MIN_WAVE_LEN) / WAVE_LEN_INC

class ColorPatch {

public:
	ColorPatch();
	//ColorPatch(std::string name, int size = ((MAX_WAVE_LEN-MIN_WAVE_LEN)/WAVE_LEN_INC));
	~ColorPatch();
	friend std::ostream& operator<<(std::ostream& os, const ColorPatch& cp) {
		os << cp.name_m << std::endl;
		for (int i = 0; i < REFLECTANCE_SIZE; i++) {
			os << "\t" << cp.reflectance[i] << std::endl;
		}
		return os;
	}
	void append(double value);
	std::string get_name()const { return name_m; }
	void set_name(std::string name) { this->name_m = name; }
	double get_ref_by_index(int index);
	double get_ref_by_wavelen(int wavelength);


private:
	std::string name_m;
	double* reflectance;
	//int size_m = REFLECTANCE_SIZE;
	int index_m = 0;
};

#endif // !COLOR_PATCH_H
