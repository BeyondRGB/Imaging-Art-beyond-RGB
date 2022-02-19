#include "utils/color_converter.hpp"



double ColorConvertions::lab_f(double x) {
	if (x > 216.0 / 24389.0) {
		return pow(x, 1.0 / 3.0);
	}
	return ((24389.0 / 27.0) * x + 16) / 116.0;
}

