#include "color_convertions.hpp"

/**
* Calculates the f(x) value used for calculating
* L*,a*,b* values
* @param x: double X/Xn, Y/Yn, or Z/Zn 
*	where X,Y,Z are the x,y,z values contained by this ColorPatch
*	and Xn, Yn, Zn are the Xn, Yn, Zn from the WhitePoints
* @return: computed double
*/
double lab_f(double x);

btrgb::Lab_t btrgb::xyz_2_Lab(btrgb::XYZ_t xyz, WhitePoints* wp){
    std::cout << "Converting XYZ->L*a*b*" << std::endl;
    double Xn = wp->get_white_point(WhitePoints::ValueType::Xn);
	double Yn = wp->get_white_point(WhitePoints::ValueType::Yn);
	double Zn = wp->get_white_point(WhitePoints::ValueType::Zn);

    double fX = lab_f(xyz.x / Xn);
	double fY = lab_f(xyz.y / Yn);
	double fZ = lab_f(xyz.z / Zn);

    btrgb::Lab_t lab;
    lab.L = 116 * fY - 16;
	lab.a = 500 * (fX - fY);
	lab.b = 200 * (fY - fZ);

    return lab;
}

double lab_f(double x) {
	if (x > 216.0 / 24389.0) {
		return pow(x, 1.0 / 3.0);
	}
	return ((24389.0 / 27.0) * x + 16) / 116.0;
}

